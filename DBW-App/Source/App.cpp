#include <iostream>
#include <string>

#pragma warning(disable : 4996)

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "Core/Core.h"

#pragma comment(lib, "ws2_32.lib")

int main() 
{

    DBW::WIFI::Init();
    DBW::WIFI::Connect("192.168.1.16", 1234);
    
    std::cout << "Type messages and press ENTER to send. Type 'exit' to quit.\n";

    std::string msg;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, msg);

        if (msg == "exit") break;

        DBW::WIFI::SendMsg(msg);
    }

    DBW::WIFI::DisConnect();
    return 0;
}
