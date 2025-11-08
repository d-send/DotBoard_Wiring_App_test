#include "Core/Raylib_Implementation.h"
#include "App.h"

void DBW::MyApp::Init()
{
    tempWire.reserve(Average_Bends_Per_Wire);
    tempWire.emplace_back(Vector2(0, 0));

    NewConnectionIP[0] = '\0';
    m_TempProjectName[0] = '\0';
}

void DBW::MyApp::RegisterEvents()
{
    m_WindowWidth = GetScreenWidth();
    m_WindowHeight = GetScreenHeight();

	mousePos = GetMousePosition();
	mouseLeftClicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    mouseRightClicked = IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);
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
    //Rendering Ribbon
    Ribbon.width = GetScreenWidth();
    GuiDrawRectangle(Ribbon, 0, Color(30, 30, 30, 255), Color(30, 30, 30, 255));
    GuiDrawText(("    Project Name : " + m_ProjectName).c_str(), Ribbon, TEXT_ALIGN_LEFT, ProjectNameColor);

    if(mouseRightClicked && CheckCollisionPointRec(mousePos, Ribbon))
    {
        RibbonEditable = true;
    }
    if (RibbonEditable)
    {
        GuiTextBox(Ribbon, m_TempProjectName, 20, RibbonEditable);
    }

    if (enterPressed && RibbonEditable == true)
    {
        RibbonEditable = false;
        m_ProjectName = m_TempProjectName;
        if (m_ProjectName.size() > 0)
        {
            ProjectNameColor = GREEN;
        }
    }
    
    //Save button
    if(GuiButton(SaveButton, "save"))
    {
        SaveProject();
    }

    //Gcode generate button
    if (GuiButton(GcodeButton, "Gcode"))
    {
        GenerateWireGCode();
    }

    //Print button
    if (GuiButton(PrintButton, "Print"))
    {
        PrintWires();
    }

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
    //saves workspace settings,Dot board, wires(the points)


    if (m_ProjectName.size() > 0)
    {
        ProjectNameColor = GRAY;

        std::ofstream fout1;
        std::string Ppath = "../ProjectFiles/" + m_ProjectName + ".txt";
        fout1.open(Ppath);

        if (!fout1)
        {
            std::cout << "Error opening file! \n";
            return;
        }

        fout1 << "boardLocation_x=" << boardLocation_x << std::endl;
        fout1 << "boardLocation_y=" << boardLocation_y << std::endl;
        fout1 << "no_Holes_x=" << no_Holes_x << std::endl;
        fout1 << "no_Holes_y=" << no_Holes_y << std::endl;
        fout1 << "wireThickness=" << wireThickness << std::endl;

        fout1 << "Wires=" << std::endl;
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

    }

    else

    {
        ProjectNameColor = RED;
    }

}

bool DBW::MyApp::GenerateWireGCode()
{
    //Calculate and generate wire G-code and saves to a file
    
    if (m_ProjectName.size() > 0)
    {
        ProjectNameColor = GRAY;
        //Calculating wire segment length and the relative angles between them
        std::vector <std::vector<float>> WireGcode;//format -> length,angle,length,angle,.....length
        std::vector<float> tempWireproperty;

        for (int i = 0;i < Wires.size();i++)
        {
            WireGcode.push_back(tempWireproperty);

            for (int j = 1;j < Wires[i].size();j++)
            {
                float POtoP1_XDistance = -(Wires[i][j - 1].x - Wires[i][j].x);
                float P0toP1_YDistance = -(Wires[i][j - 1].y - Wires[i][j].y);

                float WireTravel = sqrt(powf(POtoP1_XDistance, 2) + powf(P0toP1_YDistance, 2)) / scaleFactor;

                WireGcode[i].push_back(WireTravel);

                if ((Wires[i].size() - j) > 1)
                {
                    float P2toP1_XDistance = (Wires[i][j + 1].x - Wires[i][j].x);
                    float P2toP1_YDistance = (Wires[i][j + 1].y - Wires[i][j].y);

                    float dotProduct = POtoP1_XDistance * P2toP1_XDistance + P0toP1_YDistance * P2toP1_YDistance;
                    float crossProduct = POtoP1_XDistance * P2toP1_YDistance - P0toP1_YDistance * P2toP1_XDistance;

                    float relativeAngle = -atan2f(crossProduct, dotProduct) * RAD2DEG;

                    WireGcode[i].push_back(relativeAngle);

                }
            }
        }


        //Saving the Gcode to a file
        std::ofstream fout2;
        std::string Gpath = "../GCodes/" + m_ProjectName + "_Gcode.txt";
        fout2.open(Gpath);

        if (!fout2)
        {
            std::cout << "Error opening file! \n";
            return 0;
        }

        for (int i = 0;i < WireGcode.size();i++)
        {
            fout2 << "Wire_" << i << "={";
            fout2 << WireGcode[i][0];

            for (int j = 1;j < WireGcode[i].size();j++)
            {
                fout2 << "," << WireGcode[i][j];
            }
            fout2 << "}" << std::endl;
        }

        std::cout << "Gcode Generated succesfully! \n";
    }
    else
    {
        ProjectNameColor = RED;
        return 0;
    }

    return 1;
}

void DBW::MyApp::PrintWires()
{
    if (GenerateWireGCode())
    {
        //Sends the wire G-code to the connected wire making machine for printing
        DBW::WIFI::Init();

        if (DBW::WIFI::Connect("192.168.1.16", 1234))
        {
            return;
        }

        //sending wire data to esp32
        std::ifstream fin;
        std::string Gpath = "../GCodes/" + m_ProjectName + "_Gcode.txt";
        fin.open(Gpath);
        std::stringstream buffer;
        buffer << fin.rdbuf();//read the entire file 
        std::string Gcode = buffer.str();
        fin.close();

        DBW::WIFI::SendMsg(Gcode);

        DBW::WIFI::DisConnect();
    }

      
}
    
        
        
        
        
        
        
        
        
        

        
        

        
        
        
        
        
        
        
        

        
        
        
        
        
        
        

        
        
        
        

        
        

        
        
        
        
        
        

        
        
        
        
        
        
        
        

        
        
        
        

        
        
        
        
        
        
        

        

        
        
        
        
        
        
        

        
        
        
        
        

        
        
        
        
        
        

        
        
        
        
        

        
        
        
        
        

        

        
        
        
        
        
        
        
        

        
        
        

        

        
        

        
        
        
        

        

        
        
        
        
        
        
        
        


        
        
        
        
        

        
        
        


        
        
        
        
        

        
        
        
        
        
        
        
        

        
        
        
        
        
        
        
        
        
        

        


		

        
		

		

        
		
		

		
		
		

		

		

		

		
		
		
		
		

        

        
		
		

		
		

		


		
		

		

		

        
		

        
        
        

        
        
        
        
        

        
        
        

        
        
        
        
        
        

        

        
        
        
        
        

        
        
        
        

        

        

        
        
        
        

        
        

        

        

        
        
        

        
        
        

        
        
        
        
        

        
        
        
        

        
        
        
        
        
        

		

       