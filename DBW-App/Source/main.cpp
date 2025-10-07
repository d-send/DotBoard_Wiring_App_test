#include "App.h"




int main() 
{
    /*
    DBW::WIFI::Init();
    DBW::WIFI::Connect("192.168.1.16", 1234);
    DBW::WIFI::SendMsg("Hello world!");
    DBW::WIFI::DisConnect();
   
    std::cout << "Type messages and press ENTER to send. Type 'exit' to quit.\n";

    std::string msg;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, msg);

        if (msg == "exit") break;

        DBW::WIFI::SendMsg(msg);
    }
    */



    DBW::MyApp app(1280, 720, "DBW", 60);
    app.Run();


    
    
    

    

    /*
    //sending wire data to esp32
    std::ifstream fin;
    fin.open("../Files/WiresCoordinates.txt");
    std::stringstream buffer;
    buffer << fin.rdbuf();//read the entire file 
    std::string wireFile = buffer.str();
    fin.close();

    DBW::WIFI::SendMsg(wireFile);
    DBW::WIFI::DisConnect();
    */
    return 0;
}
