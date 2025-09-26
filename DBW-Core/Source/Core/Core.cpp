#include "Core.h"

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

namespace DBW {

	namespace WIFI {

        static SOCKET sock;

        void Init()
        {
            WSADATA wsaData;
            WSAStartup(MAKEWORD(2, 2), &wsaData);

            sock = socket(AF_INET, SOCK_STREAM, 0);
        }

		int Connect(const std::string& IP, int port)
		{
            
            if (sock == INVALID_SOCKET) {
                std::cerr << "Socket creation failed.\n";
                WSACleanup();
                return 1;
            }

            sockaddr_in addr{};
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            inet_pton(AF_INET, IP.c_str(), &addr.sin_addr);

            if (connect(sock, (sockaddr*)&addr, sizeof(addr)) != 0) {
                std::cerr << "Failed to connect to ESP32.\n";
                closesocket(sock);
                WSACleanup();
                return 1;
            }

            std::cout << "Connected to ESP32 at " << IP << ":" << port << std::endl;
            return 0;
		}

        int SendMsg(const std::string& msg)
        {
            std::string toSend = msg + "\n"; // newline so ESP32 readStringUntil('\n') works
            return send(sock, toSend.c_str(), (int)toSend.size(), 0);
        }

        int DisConnect()
        {
            int cs = closesocket(sock);
            WSACleanup();
            std::cout << "Disconnected.\n";
            return cs;
        }
	}
}

