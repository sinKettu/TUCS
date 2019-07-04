#include "server.h"
#include <unistd.h>
#include <iostream>
#include <string>

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

bool Server::Serve()
{
    fd_set rfds, wfds, efds;
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_ZERO(&efds);
    FD_SET(STDIN_FILENO, &rfds);
    int n = 0;
    std::vector<int>::iterator soc;
    for (soc = connectedSockets.begin(); soc != connectedSockets.end(); soc++)
    {
        n = std::max(n, *soc);
        FD_SET(*soc, &rfds);
        FD_SET(*soc, &wfds);
        FD_SET(*soc, &efds);
    }

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 500;
    n++;

    int res = select(n, &rfds, &wfds, &efds, &tv);
    if (res <= 0)
        return true;
    
    for (soc = connectedSockets.begin(); soc != connectedSockets.end(); soc++)
    {
        // The first is writing to answer anyway
        if (FD_ISSET(*soc, &wfds))
        {
            // Answer(*soc);
        }
        // The second is receive user command (waiting for 'exit')
        else if (FD_ISSET(*soc, &rfds) && *soc == STDIN_FILENO)
        {
            char command[6]; // "exit\n\0"
            int c_res = read(STDIN_FILENO, command, 5);
            if (c_res == 6 && !strcmp(command, "exit\n")) // ???
            {
                return false;
            }
        }
        else if (FD_ISSET(*soc, &rfds) && *soc != STDIN_FILENO)
        {
            char buf[1024] = {0};
            int len = read(*soc, buf, 1024);
            if (len > 0)
            {
                // Process(*soc, buf, len);
            }
        }
        else if (FD_ISSET(*soc, &efds))
        {
            std::cerr << "Error at socket " << *soc << std::endl;
        }
    }

    return true;
}