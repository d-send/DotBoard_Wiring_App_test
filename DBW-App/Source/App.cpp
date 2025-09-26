#include <iostream>1
#include <string>
#include <vector>

#pragma warning(disable : 4996)

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "Core/Core.h"

#pragma comment(lib, "ws2_32.lib")

#define Pitch_Between_Pins 2.54  //in mm
#define Hole_Diameter 1 //in mm

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

    std::vector<Hole> Wire;
    float wireThickness = 1.0f;//in mm

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        Vector2 mousePos = GetMousePosition();

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

        //Creating a basic Wire
        if (hover == true && IsKeyPressed(KEY_A))
        {
            Wire.push_back(holePos);
        }

        //Drawing the Wires
        for (int i = 1;i<Wire.size();i++)
        {
            DrawLineEx(Wire[i - 1], Wire[i], wireThickness * factor, RED);
        }

        if (Wire.size() > 0)
        {
            DrawLineEx(Wire.back(), mousePos, wireThickness * factor, RED);
        }
        


        EndDrawing();
    }
    CloseWindow();

   
    return 0;
}
