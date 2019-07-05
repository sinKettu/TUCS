#include "tcp_server.h"
#include <unistd.h>
#include <iostream>
#include <string>

TcpServer::TcpServer(unsigned short port)
{
    if (port <= 1000)
    {
        std::cout << "Please choose port value greater than 1000!\n";
        std::cout << "Exiting\n";
        exit(0);
    }

    serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == -1)
    {
        std::cout << "Couldn't create TCP socket!\n";
        std::cout << "Exiting\n";
        exit(0);
    }

    memset(&serverSocAddr, 0, sizeof(serverSocAddr));
    serverSocAddr.sin_family = AF_INET;
    serverSocAddr.sin_port = htons(port);
    serverSocAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(serverSocket, (struct sockaddr*) &serverSocAddr, sizeof(serverSocAddr)) == -1)
    {
        std::cout << "Couldn't bind TCP socket!\n";
        close(serverSocket);
        std::cout << "Exiting\n";
        exit(0);
    }

    if (listen(serverSocket, 5) == -1)
    {
        std::cout << "Couldn't set up TCP socket for listening!\n";
        close(serverSocket);
        std::cout << "Exiting\n";
        exit(0);
    }
}

TcpServer::~TcpServer()
{
    std::vector<int>::iterator soc;
    for (soc = clients.begin(); soc != clients.end(); soc++)
    {
        if (*soc > 0)
            close(*soc);
    }
    clients.clear();
}

int TcpServer::SetFDs(fd_set *reads, fd_set *writes, fd_set *exceptions)
{
    int n = Server::SetFDs(reads, writes, exceptions);
    if (n < 0)
    {
        std::cout << "Something went wrong!\n";
        return -1;
    }

    std::vector<int>::iterator iter;
    for (iter = clients.begin(); iter != clients.end(); iter++)
    {
        n = std::max(*iter, n);
        FD_SET(*iter, reads);
        FD_SET(*iter, writes);
        FD_SET(*iter, exceptions);
    }

    return n + 1;
}

bool TcpServer::GetFDs(fd_set *reads, fd_set *writes, fd_set *exceptions)
{
    if (FD_ISSET(serverSocket, reads))
    {
        // socklen_t socLen = sizeof(serverSocAddr);
        // int newSoc = accept(serverSocket, (struct sockaddr *) &serverSocAddr, &socLen);
        int newSoc = accept(serverSocket, nullptr, nullptr);
        if (newSoc <= 0)
            std::cout << "Couldn't accept pending connection\n";
        else
            clients.push_back(newSoc);
    }
    else if (FD_ISSET(serverSocket, exceptions))
    {
        std::cout << "Error at TCP server was occured!\nStopping TCP server.\n";
        return false;
    }

    std::vector<int>::iterator soc;
    for (soc = clients.begin(); soc != clients.end(); soc++)
    {
        // Main priority is give response
        if (FD_ISSET(*soc, writes) && !resp.empty() && resp.front().first == *soc)
        {
            Write(*soc, resp.front().second);
            resp.pop();
        }
        else if (FD_ISSET(*soc, reads))
        {
            std::string request = Read(*soc);
            std::string processed = ProcessRequest(request);
            resp.push(std::make_pair(*soc, processed));
        }
        else if (FD_ISSET(*soc, exceptions))
        {
            std::cout << "Error at socket '" << *soc << "' was occured or socket disconnected!\nClosing socket.\n";
            clients.erase(soc--); // ???
        }
    }

    return true;
}
