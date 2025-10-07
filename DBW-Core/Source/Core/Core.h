#pragma once

#include <string>

namespace DBW {

	class App
	{
	public:
		virtual void RegisterEvents()=0;
		virtual void ExecuteEvents()=0;
		virtual void Render()=0;
		virtual void RenderOverlays() = 0;
		virtual void Run()=0;


	public:
		App() {};
		~App() {};
	};

	namespace WIFI {

		void Init();
		int Connect(const std::string& IP, int port);
		int SendMsg( const std::string& msg);
		int DisConnect();
	}

}