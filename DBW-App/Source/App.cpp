
#include "Core/Core.h"

#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")

int main() {
    DBW::WIFI::Init();

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    DBW::WIFI::Connect(sock,"192.168.1.16", 1234);
    
    std::cout << "Type messages and press ENTER to send. Type 'exit' to quit.\n";

    std::string msg;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, msg);

        if (msg == "exit") break;

        DBW::WIFI::SendMsg(sock, msg);
    }

    DBW::WIFI::DisConnect(sock);
    return 0;
}
