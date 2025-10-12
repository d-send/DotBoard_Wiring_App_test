#include "Core/Raylib_Implementation.h"
#include "App.h"

void DBW::MyApp::Init()
{
    tempWire.reserve(Average_Bends_Per_Wire);
    tempWire.emplace_back(Vector2(0, 0));

    NewConnectionIP[0] = '\0';
}

void DBW::MyApp::RegisterEvents()
{
	mousePos = GetMousePosition();
	mouseLeftClicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
	escPressed = IsKeyPressed(KEY_ESCAPE);
	enterPressed = IsKeyPressed(KEY_ENTER);
    deletePressed = IsKeyPressed(KEY_DELETE);
    KeyAPressed = IsKeyPressed(KEY_A);
    KeyEPressed = IsKeyPressed(KEY_E);
    KeyXPressed = IsKeyPressed(KEY_X);
}
	
void DBW::MyApp::UpdateandRender()
{

    BeginDrawing();
    ClearBackground(WHITE);

    pitch = Pitch_Between_Pins * scaleFactor;

    //Rendering Dot Board
    RenderDotBoard(no_Holes_x, no_Holes_y, boardLocation_x, boardLocation_y, BROWN, WHITE);
    //

    //Detecting if the mouse pointer is hovering over a hole
    IsMousePointerHoveringOverHole();
    //

    //delecting selected wire
    if (SelectedWire != -1 && deletePressed)
    {
        Wires.erase(Wires.begin() + SelectedWire);
        SelectedWire = -1;
    }

    //Creating a new wire
    if (isHoveringOverHole == true && KeyAPressed)
    {
        tempWire[0] = hoveringHolePos;
        Wires.emplace_back(tempWire);
        wireAdded = true;
        wireExtendable = true;
    }

    //Selecting a wire
    HoveringWire = -1;
    for (int i = 0;i < Wires.size();i++)
    {

        for (int j = 1;j < Wires[i].size();j++)
        {
            //mouse pointer is just over the wire
            if (CheckCollisionPointLine(mousePos, Wires[i][j - 1], Wires[i][j], wireThickness * scaleFactor))
            {
                HoveringWire = i;
            }

        }

        //mouse pointer is over the wire and user left clicked to select
        if (mouseLeftClicked && HoveringWire != -1)
        {
            SelectedWire = i;
            break;
        }
    }

    //mouse pointer is not over a wire and user left clicked to deselect
    if (mouseLeftClicked && HoveringWire == -1)
    {
        SelectedWire = -1;
    }

    //extending the wire
    if (isHoveringOverHole == true && wireExtendable == true && KeyEPressed)
    {
        Wires.back().emplace_back(hoveringHolePos);
        wireAdded = false;
    }

    //deleting the wire if it has no length
    if (wireAdded == true && KeyXPressed)
    {
        Wires.pop_back();
    }

    //terminating the wire with length
    if (wireExtendable == true && KeyXPressed)
    {
        wireTerminated = true;
        wireExtendable = false;

    }

    //drawing the Wires
    Color WireColor = RED;
    for (int i = 0;i < Wires.size();i++)
    {
        if (i == SelectedWire || i == HoveringWire)
        {
            WireColor = MAROON;
        }

        for (int j = 1;j < Wires[i].size();j++)
        {
            DrawLineEx(Wires[i][j - 1], Wires[i][j], wireThickness * scaleFactor, WireColor);
        }

        WireColor = RED;
    }

    //drawing the extendable wire part
    if (Wires.size() > 0 && wireExtendable == true)
    {
        DrawLineEx(Wires.back().back(), mousePos, wireThickness * scaleFactor, RED);
    }
}
		
void DBW::MyApp::UpdateandRenderOverlays()
{
    //Connection select drop down menu
    if (!NewConnectionEditable)
    {
        if (GuiDropdownBox(PastconnectionsMenu, Pastconnections.c_str(), &connectionIndex, connectionEditMode))
        {
            connectionEditMode = !connectionEditMode; // Toggle edit mode on click
        }
    }


    if (connectionIndex == (NoofItems - 1))
    {
        NewConnectionEditable = true;
        NewConnectionExit = false;
    }

    if (NewConnectionEditable)
    {
        GuiTextBox(PastconnectionsMenu, NewConnectionIP, sizeof(NewConnectionIP), NewConnectionEditable);


        if (enterPressed)
        {
            NewConnectionEditable = false;
            NewConnectionAdd = true;
        }

        if (escPressed)
        {
            NewConnectionEditable = false;
            NewConnectionExit = true;
            connectionIndex = connectionIndex - 1;
            NewConnectionIP[0] = '\0';
        }
    }

    if (NewConnectionAdd)
    {
        //TODO:Check validity fo the IP
        NoofItems++;
        Pastconnections.erase(Pastconnections.end() - 10, Pastconnections.end());
        Pastconnections = Pastconnections + NewConnectionIP + ";Add New...";
        std::cout << Pastconnections << std::endl;
        NewConnectionIP[0] = '\0';
        NewConnectionAdd = false;
    }
}

void DBW::MyApp::Run()
{


    while (!WindowShouldClose())
    {
        RegisterEvents();

        UpdateandRender();

        UpdateandRenderOverlays();

        EndDrawing();
    }

    CloseWindow();
}

DBW::MyApp::MyApp(int WindowWidth, int WindowHeight, const char* AppName, int TargetFPS)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(WindowWidth, WindowHeight, AppName);
    SetTargetFPS(TargetFPS);

    SetExitKey(KEY_NULL);

}
		
		
DBW::MyApp::~MyApp()
{

    //Saving the Wires Coordinates to a file
    std::ofstream fout1;
    fout1.open("../WireFiles/WiresCoordinates.txt");
    
    if (!fout1)
    {
        std::cout << "Error opening file! \n";
        return;
    }

    for (int i = 0;i < Wires.size();i++)
    {
        fout1 << "Wire_" << i << "={";

        for (int j = 0;j < Wires[i].size();j++)
        {
            fout1 << "[" << Wires[i][j].x << "," << Wires[i][j].y << "]";
        }
        fout1 << "}" << std::endl;
    }

    fout1.close();

    //Calculating wire segment length and the relative angles between them
    std::vector <std::vector<float>> Wireproperties;//format -> length,angle,length,angle,.....length
    for (int i = 0;i < Wires.size();i++)
    {
        Wireproperties.push_back(tempWireproperty);

        for (int j = 1;j < Wires[i].size();j++)
        {
            float POtoP1_XDistance = -(Wires[i][j - 1].x - Wires[i][j].x);
            float P0toP1_YDistance = -(Wires[i][j - 1].y - Wires[i][j].y);

            float WireTravel = sqrt(powf(POtoP1_XDistance, 2) + powf(P0toP1_YDistance, 2)) / scaleFactor;

            Wireproperties[i].push_back(WireTravel);

            if ((Wires[i].size() - j) > 1)
            {
                float P2toP1_XDistance = (Wires[i][j + 1].x - Wires[i][j].x);
                float P2toP1_YDistance = (Wires[i][j + 1].y - Wires[i][j].y);

                float dotProduct = POtoP1_XDistance * P2toP1_XDistance + P0toP1_YDistance * P2toP1_YDistance;
                float crossProduct = POtoP1_XDistance * P2toP1_YDistance - P0toP1_YDistance * P2toP1_XDistance;

                float relativeAngle = -atan2f(crossProduct, dotProduct) * RAD2DEG;

                Wireproperties[i].push_back(relativeAngle);

            }
        }
    }

    //Saving the Wires properties to a file
    std::ofstream fout2;
    fout2.open("../GCodes/Gcode.txt");

    if (!fout2)
    {
        std::cout << "Error opening file! \n";
        return;
    }

    for (int i = 0;i < Wireproperties.size();i++)
    {
        fout2 << "Wire_" << i << "={";
        fout2 << Wireproperties[i][0];

        for (int j = 1;j < Wireproperties[i].size();j++)
        {
            fout2 << "," << Wireproperties[i][j];
        }
        fout2 << "}" << std::endl;
    }
}

void DBW::MyApp::RenderDotBoard(int No_Holes_x, int No_Holes_y, int BoardLocation_x, int BoardLocation_y, Color BoardColor, Color HoleColor)
{
    DrawRectangle(BoardLocation_x - pitch, BoardLocation_y - pitch, pitch * (No_Holes_y + 1), pitch * (No_Holes_x + 1), BoardColor);
    for (int i = 0;i < No_Holes_x;i++)
    {
        for (int j = 0;j < No_Holes_y;j++)
        {
            DrawCircle(BoardLocation_x + pitch * j, BoardLocation_y + pitch * i, Hole_Diameter * 0.5f * scaleFactor, HoleColor);
        }
    }
}

bool DBW::MyApp::IsMousePointerHoveringOverHole()
{
    isHoveringOverHole = false;
    for (int i = 0;i < no_Holes_x * no_Holes_y;i++)
    {
        for (int j = 0;j < no_Holes_y;j++)
        {
            hoveringHolePos = { boardLocation_x + pitch * j, boardLocation_y + pitch * i };

            if (CheckCollisionPointCircle(mousePos, hoveringHolePos, Hole_Diameter * 1.0f * scaleFactor))
            {
                isHoveringOverHole = true;
                DrawCircleLines(boardLocation_x + pitch * j, boardLocation_y + pitch * i, Hole_Diameter * 1.0f * scaleFactor, GREEN);
                break;
            }
        }

        if (isHoveringOverHole == true)
        {
            break;
        }
    }

    return isHoveringOverHole;
}
        
void DBW::MyApp::SaveProject()
{

}

void DBW::MyApp::SaveasProject()
{

}

void DBW::MyApp::GenerateWireGCode()
{

}

void DBW::MyApp::PrintWires()
{

}
    
        
        
        
        
        
        
        
        
        

        
        

        
        
        
        
        
        
        
        

        
        
        
        
        
        
        

        
        
        
        

        
        

        
        
        
        
        
        

        
        
        
        
        
        
        
        

        
        
        
        

        
        
        
        
        
        
        

        

        
        
        
        
        
        
        

        
        
        
        
        

        
        
        
        
        
        

        
        
        
        
        

        
        
        
        
        

        

        
        
        
        
        
        
        
        

        
        
        

        

        
        

        
        
        
        

        

        
        
        
        
        
        
        
        


        
        
        
        
        

        
        
        


        
        
        
        
        

        
        
        
        
        
        
        
        

        
        
        
        
        
        
        
        
        
        

        


		

        
		

		

        
		
		

		
		
		

		

		

		

		
		
		
		
		

        

        
		
		

		
		

		


		
		

		

		

        
		

        
        
        

        
        
        
        
        

        
        
        

        
        
        
        
        
        

        

        
        
        
        
        

        
        
        
        

        

        

        
        
        
        

        
        

        

        

        
        
        

        
        
        

        
        
        
        
        

        
        
        
        

        
        
        
        
        
        

		

       