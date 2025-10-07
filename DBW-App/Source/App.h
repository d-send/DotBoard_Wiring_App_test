#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <math.h>

#pragma warning(disable : 4996)

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
			void RegisterEvents();
			void ExecuteEvents();
			void Render();
			void RenderOverlays();
			void Run();


		public:
			MyApp();
			MyApp(int WindowWidth, int WindowHeight, const char* AppName, int TargetFPS);
			~MyApp();

		private:
			Vector2 mousePos = { 0,0 };
			bool mouseLeftClicked = false;
			bool escPressed = false;
			bool enterPressed = false;

			float factor = 8.8f;

			//top left corner
			int boardLocation_x = 25;
			int boardLocation_y = 50;

			int No_Holes_x = 24;
			int No_Holes_y = 56;

			bool hover = false;

			std::vector<std::vector<Hole>> Wires;

			std::vector<Hole> tempWire;
			std::vector<float> tempWireproperty;

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
			bool movebend = false;
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
		};
}