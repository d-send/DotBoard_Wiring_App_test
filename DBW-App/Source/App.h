#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <math.h>

#include "Core/Raylib_Headers.h"
#include "Core/Core.h"

#define Pitch_Between_Pins 2.54  //in mm
#define Hole_Diameter 1 //in mm
#define Average_Bends_Per_Wire 10

typedef Vector2 Hole;

namespace DBW 
{
		class MyApp : public App
		{
		public:
			void Init();
			void RegisterEvents();
			void UpdateandRender();
			void UpdateandRenderOverlays();
			void Run();


		public:
			MyApp(int WindowWidth, int WindowHeight, const char* AppName, int TargetFPS);
			~MyApp();

		private:
			void RenderDotBoard(int No_Holes_x,int No_Holes_y, int BoardLocation_x,int BoardLocation_y,Color BoardColor,Color HoleColor);
			bool IsMousePointerHoveringOverHole();
			void SaveProject();//saves workspace settings,Dot board, wires(the points)//also saves new connections added to a seperate file
			bool GenerateWireGCode();//Calculate and generate wire G-code and saves to a file
			void PrintWires();//Sends the wire G-code to the connected wire making machine for printing
			

		private:
			//member variables
			int m_WindowWidth;
			int m_WindowHeight;

			float pitch = 0.0f;
			float scaleFactor = 8.8f;

			//top left corner
			int boardLocation_x = 25;
			int boardLocation_y = 50;

			int no_Holes_x = 24;
			int no_Holes_y = 56;

			Rectangle Ribbon = { 0, 0, 0, 30 };
			std::string m_ProjectName;
			char m_TempProjectName[20];
			Color ProjectNameColor = GRAY;
			bool RibbonEditable = false;

			Rectangle SaveButton = { 400, 50, 100, 30 };

			Rectangle GcodeButton = { 550, 50, 100, 30 };

			Rectangle PrintButton = { 700, 50, 100, 30 };

		private:
			//variables for registering events
			Vector2 mousePos = { 0,0 };
			bool mouseLeftClicked = false;
			bool mouseRightClicked = false;
			bool escPressed = false;
			bool enterPressed = false;
			bool deletePressed = false;
			bool KeyAPressed = false;
			bool KeyEPressed = false;
			bool KeyXPressed = false;

			bool isHoveringOverHole = false;
			Vector2 hoveringHolePos = { 0,0 };

			std::vector<std::vector<Hole>> Wires;//Node locations

			std::vector<Hole> tempWire;

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
			int HoveringWire = -1;
			int SelectedWire = -1;

			Rectangle PastconnectionsMenu = { 100, 50, 200, 30 };
			int connectionIndex = 0;
			bool connectionEditMode = false;
			std::string Pastconnections = "None;Add New...";
			int NoofItems = 2;
			char NewConnectionIP[16];

			bool NewConnectionEditable = false;
			bool NewConnectionExit = false;
			bool NewConnectionAdd = false;

			bool isProjectSaved = false;
		};
}