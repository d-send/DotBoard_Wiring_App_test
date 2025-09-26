#pragma once

#include <string>

namespace DBW {

	namespace WIFI {

		void Init();
		int Connect(const std::string& IP, int port);
		int SendMsg( const std::string& msg);
		int DisConnect();
	}

}