#include "udp_server.h"
#include <iostream>
#include <unistd.h>

UdpServer::UdpServer(unsigned short port)
{
    if (port <= 1000)
    {
        std::cout << "Please choose port value greater than 1000!\n";
        std::cout << "Exiting\n";
        exit(1);
    }

    serverSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSocket == -1)
    {
        std::cout << "Couldn't create UDP socket!\n";
        std::cout << "Exiting\n";
        exit(1);
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
        exit(1);
    }
}

bool UdpServer::GetFDs(fd_set *reads, fd_set *writes, fd_set *exceptions)
{
    bool res = true;
    if (serverSocket <= 0)
        return false;
    
    if (FD_ISSET(serverSocket, reads))
    {
        // UDP responds immediately
        ReadProcessWrite(serverSocket);
    }
    
    if (FD_ISSET(serverSocket, exceptions))
    {
        std::cout << "Error at UDP server was occured!\nStopping UDP server.\n";
        res = false;
    }

    return res;
}

void UdpServer::ReadProcessWrite(int soc)
{
    // Read part

    if (soc <= 0)
    {
        return;
    }
    
    char buf[1024];
    sockaddr sa;
    socklen_t sa_l = sizeof(sa);
    memset(&sa, 0, sizeof(sa));
    int res = recvfrom(soc, buf, 1024, 0, &sa, &sa_l);
    if (res <= 0)
    {
        std::cout << "Unsuccessful attempt to receive data from UDP socket\n";
        return;
    }
    std::cout << "Data was received at UDP socket\n";

    // Process part

    std::string tmp(buf, res);
    tmp = ProcessRequest(tmp);

    // Write part

    if (tmp.length() > 1024)
    {
        std::cout << "Too long data to write to UDP socket\n";
        return;
    }

    res = sendto(soc, tmp.c_str(), tmp.length(), 0, &sa, sa_l);
    if (res <= 0)
        std::cout << "Couldn't write data to UDP socket\n";
    else
        std::cout << "Data was successfully written to UDP socket\n";
    
}