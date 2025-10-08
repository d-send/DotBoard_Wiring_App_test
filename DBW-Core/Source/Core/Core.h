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

	namespace WIFI {

		void Init();
		int Connect(const std::string& IP, int port);
		int SendMsg( const std::string& msg);
		int DisConnect();
	}

}