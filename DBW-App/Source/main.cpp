#include "App.h"

int main() 
{
    DBW::MyApp app(1280, 720, "DBW", 60);

    app.Init();
    app.Run();

    return 0;
}
