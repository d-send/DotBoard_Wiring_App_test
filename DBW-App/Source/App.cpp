#include <iostream>1
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <math.h>

#pragma warning(disable : 4996)

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "Core/Core.h"



class MyApp : public DBW::App
{
	public:
		void RegisterEvents()
		{
			mousePos = GetMousePosition();
			mouseLeftClicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
			escPressed = IsKeyPressed(KEY_ESCAPE);
			enterPressed = IsKeyPressed(KEY_ENTER);
		}

		void ExecuteEvents()
		{

		}

		void Render()
		{
			
			BeginDrawing();
			ClearBackground(WHITE);

			


		}

		void RenderOverlays()
		{

		}

		void Run()
		{
			

			while (!WindowShouldClose())
			{
				RegisterEvents();

				ExecuteEvents();

				Render();

				RenderOverlays();

				EndDrawing();
			}
			
			CloseWindow();
		}

	public:

		MyApp(int WindowWidth,int WindowHeight,const char* AppName,int TargetFPS)
		{
			SetConfigFlags(FLAG_WINDOW_RESIZABLE);

			InitWindow(WindowWidth, WindowHeight, AppName);
			SetTargetFPS(TargetFPS);

			SetExitKey(KEY_NULL);
		}

		~MyApp()
		{

		}

	private:
		Vector2 mousePos = {0,0};
		bool mouseLeftClicked = false;
		bool escPressed = false;
		bool enterPressed = false;

};
