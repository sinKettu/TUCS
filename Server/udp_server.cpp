#include "udp_server.h"
#include <iostream>
#include <unistd.h>

UdpServer::UdpServer(unsigned short port)
{
    if (port <= 1000)
    {
        std::cout << "Please choose port value greater than 1000!\n";
        std::cout << "Exiting\n";
        exit(0);
    }

    serverSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSocket == -1)
    {
        std::cout << "Couldn't create UDP socket!\n";
        std::cout << "Exiting\n";
        exit(0);
    }

    memset(&serverSocAddr, 0, sizeof(serverSocAddr));
    serverSocAddr.sin_family = AF_INET;
    serverSocAddr.sin_port = htons(port);
    serverSocAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(serverSocket, (struct sockaddr*) &serverSocAddr, sizeof(serverSocAddr)) == -1)
    {
        std::cout << "Couldn't bind UDP socket!\n";
        close(serverSocket);
        std::cout << "Exiting\n";
        exit(0);
    }

    if (listen(serverSocket, 5) == -1)
    {
        std::cout << "Couldn't set up UDP socket for listening!\n";
        close(serverSocket);
        std::cout << "Exiting\n";
        exit(0);
    }
}

bool UdpServer::GetFDs(fd_set *reads, fd_set *writes, fd_set *exceptions)
{
    if (FD_ISSET(serverSocket, reads))
    {
        // UDP responds immediately
        std::string request = Read(serverSocket);
        std::string processed = ProcessRequest(request);
        write(serverSocket, processed.c_str(), processed.length());
        return true;
    }
    else if (FD_ISSET(serverSocket, exceptions))
    {
        std::cout << "Error at UDP server was occured!\nStopping UDP server.\n";
        return false;
    }
}