#include "server.h"
#include <iostream>

int main()
{
    std::cout << "Hello!\n";
    std::cout << "Test TCP & UDP server is starting...\n";
    Server server;
    
    if (!server.Init())
    {
        std::cout << "Bye!\n";
        return 1;
    }

    std::cout << "Success!\n";

    while (server.Serve());
    std::cout << "Bye!\n";
    return 0;
}