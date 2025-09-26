#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

namespace DBW {

	namespace WIFI {

		void Init();
		int Connect(SOCKET& sock,const std::string& IP, int port);
		int SendMsg(SOCKET& sock, const std::string& msg);
		int DisConnect(SOCKET& sock);
	}

}