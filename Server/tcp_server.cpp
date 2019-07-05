#include "tcp_server.h"
#include <unistd.h>
#include <iostream>
#include <string>
#include <fcntl.h>

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
    fcntl(serverSocket, F_SETFL, O_NONBLOCK);

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
        int newSoc = accept(serverSocket, nullptr, nullptr);
        if (newSoc <= 0)
            std::cout << "Couldn't accept pending connection\n";
        else
        {
            std::cout << "New client at socket '" << newSoc << "'\n";
            clients.push_back(newSoc);
        }
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
            if (Write(*soc, resp.front().second))
                std::cout << "Data was send to socket '" << *soc << "'\n";
            else
                std::cout << "Couldn't send data to socket '" << *soc << "'\n";

            resp.pop();
        }
        else if (FD_ISSET(*soc, reads))
        {
            std::string request = Read(*soc);
            if (request.empty())
            {
                std::cout << "Seems like socket '" << *soc << "' was disconnected or something went wrong\n";
                close(*soc);
                clients.erase(soc--);
            }
            else
            {
                std::cout << "Data was read from socket '" << *soc << "'\n";
                std::string processed = ProcessRequest(request);
                resp.push(std::make_pair(*soc, processed));
            }
        }
        else if (FD_ISSET(*soc, exceptions))
        {
            std::cout << "Error at socket '" << *soc << "' was occured!\nClosing socket.\n";
            close(*soc);
            clients.erase(soc--);
        }
    }

    return true;
}

bool TcpServer::Write(int soc, std::string response)
{
    if (soc <= 0 || response.length() > 1024)
        return false;

    int res = write(soc, response.c_str(), response.length());
    return res == static_cast<int>(response.length());
}

std::string TcpServer::Read(int soc)
{
    if (soc <= 0)
        return "";
    
    char buf[1024];
    int res = read(soc, buf, 1024);
    if (res > 0)
        return std::string(buf, res) + "\0";
    else
        return "";
}