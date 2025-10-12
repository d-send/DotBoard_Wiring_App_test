#pragma once

#include <string>

namespace DBW {

	class App
	{
	public:
		virtual void Init() = 0;
		virtual void RegisterEvents()=0;
		virtual void UpdateandRender()=0;
		virtual void UpdateandRenderOverlays() = 0;
		virtual void Run()=0;


	public:
		App() {};
		~App() {};
	};

	/*
	class File
	{
	private:
		std::string Location;
		std::ofstream file;

	public:
		inline void Close();
		inline void operator<<(int val);
		inline void operator<<(float val);
		inline void operator<<(const char* text);
		inline void operator<<(const std::string& text);

	public:
		File(const char* Location);
		File(const std::string& Location);
		~File();
		
	};
	*/

	namespace WIFI {

		void Init();
		int Connect(const std::string& IP, int port);
		int SendMsg( const std::string& msg);
		int DisConnect();
	}

	

}