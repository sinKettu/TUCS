#pragma once
#include "server.h"

typedef std::queue<std::pair<int, std::string>> Responses;

class TcpServer : public Server
{

private:
    // nothing

public:
    TcpServer(unsigned short port = TCP_PORT);
    ~TcpServer();
    int SetFDs(fd_set *reads, fd_set *writes, fd_set *exceptions);
    bool GetFDs(fd_set *reads, fd_set *writes, fd_set *exceptions);

protected:
    Responses resp;
    std::vector<int> clients;

    bool Write(int soc, std::string response);
    std::string Read(int soc);
};