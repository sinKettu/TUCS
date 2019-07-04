#include "server.h"
#include <unistd.h>
#include <iostream>

bool Server::Init()
{
    tcpSoc = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (tcpSoc == -1)
    {
        std::cout << "Couldn't create TCP socket!\n";
        return false;
    }

    udpSoc = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSoc == -1)
    {
        std::cout << "Couldn't create UDP socket!\n";
        return false;
    }

    memset(&tcpSockAddr, 0, sizeof(tcpSockAddr));
    tcpSockAddr.sin_family = AF_INET;
    tcpSockAddr.sin_port = htons(TCP_PORT);
    tcpSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(tcpSoc, (struct sockaddr*) &tcpSockAddr, sizeof(tcpSockAddr)) == -1)
    {
        std::cout << "Couldn't bind TCP socket!\n";
        close(tcpSoc);
        return false;
    }

    memset(&udpSockAddr, 0, sizeof(udpSockAddr));
    udpSockAddr.sin_family = AF_INET;
    udpSockAddr.sin_port = htons(UDP_PORT);
    udpSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(udpSoc, (struct sockaddr*) &udpSockAddr,  sizeof(udpSockAddr)) == -1)
    {
        std::cout << "Couldn't bind UDP socket!\n";
        close(udpSoc);
        close(tcpSoc);
        return false;
    }

    if (listen(tcpSoc, 5) == -1)
    {
        std::cout << "Couldn't set up TCP socket for listening!\n";
        close(tcpSoc);
        close(udpSoc);
        return false;
    }

    return true;
}