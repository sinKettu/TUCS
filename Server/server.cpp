#include "server.h"
#include <unistd.h>
#include <iostream>

Server::~Server()
{
    if (serverSocket > 0)
        close(serverSocket);
    
    Responses tmp;
    std::swap(resp, tmp);
    memset(&serverSocAddr, 0, sizeof(serverSocAddr));
}

void Server::SetFDs(fd_set *reads, fd_set *writes, fd_set *exceptions)
{
    if (serverSocket <= 0)
        return;
    
    FD_SET(serverSocket, reads);
    FD_SET(serverSocket, writes);
    FD_SET(serverSocket, exceptions);
}

bool Server::Write(int soc, std::string response)
{
    if (soc <= 0 || response.length() > 1024)
        return false;

    int res = write(soc, response.c_str(), response.length());
    return res == static_cast<int>(response.length());
}

std::string Server::Read(int soc)
{
    if (soc <= 0)
        return "";
    
    char buf[1024];
    int res = read(soc, buf, 1024);
    if (res > 0)
        return std::string(buf, res);
    else
        return "";
}