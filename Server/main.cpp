#include "server.h"
#include <stdio.h>

int main()
{
    printf("Hello!\n");
    printf("Test TCP & UDP server is starting...\n");
    Server server;
    
    if (!server.Init())
    {
        printf("Bye!\n");
        return 1;
    }

    printf("Success!\n");

    while (server.Serve());

    return 0;
}