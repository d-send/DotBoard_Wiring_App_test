#include <iostream>1
#include <string>
#include <vector>
#include <fstream>

#pragma warning(disable : 4996)

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "Core/Core.h"

#pragma comment(lib, "ws2_32.lib")

#define Pitch_Between_Pins 2.54  //in mm
#define Hole_Diameter 1 //in mm
#define Average_Bends_Per_Wire 10

typedef Vector2 Hole;

int main() 
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280,720, "DBW");
    SetTargetFPS(60);

    /*
    DBW::WIFI::Init();
    DBW::WIFI::Connect("192.168.1.16", 1234);
    DBW::WIFI::SendMsg("Hello world!");
    DBW::WIFI::DisConnect();
   
    std::cout << "Type messages and press ENTER to send. Type 'exit' to quit.\n";

    std::string msg;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, msg);

        if (msg == "exit") break;

        DBW::WIFI::SendMsg(msg);
    }
    */

    float factor = 8.8f;

    //top left corner
    int boardLocation_x = 25;
    int boardLocation_y = 50;

    int No_Holes_x = 24;
    int No_Holes_y = 56;

    bool hover = false;

    std::vector<std::vector<Hole>> Wires;
    std::vector<Hole> tempWire;
    tempWire.reserve(Average_Bends_Per_Wire);
    tempWire.emplace_back(Vector2(0, 0));

    float wireThickness = 1.0f;//in mm

    //Wire Controls
    //A - Add new wire
    //X - Terminate current wire
    //E - Extend the Wire 
    //Delete - Deleting Selected wire
    //C - Move bend

    bool wireAdded = false;
    bool wireTerminated = false;
    bool wireExtendable = false;
    bool Movebend = false;
    int HoveringWire = -1;
    int SelectedWire = -1;


    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        Vector2 mousePos = GetMousePosition();
        bool mouseLeftClicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        //scrolling for zooming in and out
        //factor = factor + GetMouseWheelMove()*0.4;

        //Dot Board
        float pitch = Pitch_Between_Pins * factor;
        DrawRectangle(boardLocation_x- pitch, boardLocation_y- pitch, pitch * (No_Holes_y+1), pitch * (No_Holes_x+1),BROWN);
        for (int i = 0;i<No_Holes_x;i++)
        {
            for (int j = 0;j < No_Holes_y;j++)
            {
                DrawCircle(boardLocation_x + pitch * j , boardLocation_y + pitch * i, Hole_Diameter * 0.5f * factor, WHITE);
            }
            
        }
        //

        //Detecting if the mouse pointer is hovering over a hole
        hover = false;
        Vector2 holePos;
        for (int i = 0;i < No_Holes_x* No_Holes_y;i++)
        {
            for (int j = 0;j < No_Holes_y;j++)
            {
                holePos = { boardLocation_x + pitch * j, boardLocation_y + pitch * i };

                if (CheckCollisionPointCircle(mousePos, holePos, Hole_Diameter * 1.0f * factor))
                {
                    hover = true;
                    DrawCircleLines(boardLocation_x + pitch * j, boardLocation_y + pitch * i, Hole_Diameter * 1.0f * factor, GREEN);
                    break;
                }
            }

            if (hover == true)
            {
                break;
            }

        }
        //

        //delecting selected wire
        if (SelectedWire != -1 && IsKeyPressed(KEY_DELETE))
        {
            Wires.erase(Wires.begin()+SelectedWire);
            SelectedWire = -1;
        }

        //Creating a new wire
        if (hover == true && IsKeyPressed(KEY_A))
        {
            tempWire[0] = holePos;
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
                if (CheckCollisionPointLine(mousePos, Wires[i][j - 1], Wires[i][j], wireThickness * factor))
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
        if (hover == true && wireExtendable == true && IsKeyPressed(KEY_E))
        {
            Wires.back().emplace_back(holePos);
            wireAdded = false;
        }

        //deleting the wire if it has no length
        if (wireAdded == true && IsKeyPressed(KEY_X))
        {
            Wires.pop_back();
        }

        //terminating the wire with length
        if (wireExtendable == true && IsKeyPressed(KEY_X))
        {
            wireTerminated = true;
            wireExtendable = false;

        }
        
        //drawing the Wires
        Color WireColor = RED;
        for (int i = 0;i<Wires.size();i++)
        {
            if(i == SelectedWire || i == HoveringWire)
            {
                WireColor = MAROON;
            }

            for (int j = 1;j < Wires[i].size();j++)
            {
                DrawLineEx(Wires[i][j-1], Wires[i][j], wireThickness * factor, WireColor);

            }

            WireColor = RED;
        }
        
        if (Wires.size() > 0 && wireExtendable == true)
        {
            DrawLineEx(Wires.back().back(), mousePos, wireThickness * factor, RED);
        }
       
        EndDrawing();
    }
    CloseWindow();

    //Saving the data to a file
    std::ofstream fout;
    fout.open("../Files/Wires.txt");

    if (!fout)
    {
        std::cout << "Error opening file! \n";
        return 1;
    }

    for (int i = 0;i < Wires.size();i++)
    {
        fout << "Wire_"<<i<<"={";

        for (int j = 0;j < Wires[i].size();j++)
        {
            fout <<"["<< Wires[i][j].x<<","<<Wires[i][j].y<<"]";
        }
        fout << "}" << std::endl;
    }

    fout.close();


   
    return 0;
}
