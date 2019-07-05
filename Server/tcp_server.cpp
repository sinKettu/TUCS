#include "tcp_server.h"
#include <unistd.h>
#include <iostream>
#include <string>

TcpServer::TcpServer(unsigned short port)
{
    if (port <= 1000)
    {
        std::cout << "Please choose port value greater than 1000!\n";
        return;
    }

    serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == -1)
    {
        std::cout << "Couldn't create TCP socket!\n";
        return;
    }

    memset(&serverSocAddr, 0, sizeof(serverSocAddr));
    serverSocAddr.sin_family = AF_INET;
    serverSocAddr.sin_port = htons(port);
    serverSocAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(serverSocket, (struct sockaddr*) &serverSocAddr, sizeof(serverSocAddr)) == -1)
    {
        std::cout << "Couldn't bind TCP socket!\n";
        close(serverSocket);
        return;
    }

    if (listen(serverSocket, 5) == -1)
    {
        std::cout << "Couldn't set up TCP socket for listening!\n";
        close(serverSocket);
        return;
    }
}

void TcpServer::SetFDs(fd_set *reads, fd_set *writes, fd_set *exceptions)
{
    Server::SetFDs(reads, writes, exceptions);

    std::vector<int>::iterator iter;
    for (iter = clients.begin(); iter != clients.end(); iter++)
    {
        FD_SET(*iter, reads);
        FD_SET(*iter, writes);
        FD_SET(*iter, exceptions);
    }
}

void TcpServer::GetFDs(fd_set *reads, fd_set *writes, fd_set *exceptions)
{
    if (FD_ISSET(serverSocket, reads))
    {
        // socklen_t socLen = sizeof(serverSocAddr);
        // int newSoc = accept(serverSocket, (struct sockaddr *) &serverSocAddr, &socLen);
        int newSoc = accept(serverSocket, nullptr, nullptr);
        clients.push_back(newSoc);
    }
    else if (FD_ISSET(serverSocket, exceptions))
    {
        std::cout << "Error at TCP server was occured!\nStopping TCP server.\n";
        this->~TcpServer();
        return;
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
            std::cout << "Error at socket '" << *soc << "' was occured!\nClosing socket.\n";
            clients.erase(soc--); // ???
        }
    }
}
