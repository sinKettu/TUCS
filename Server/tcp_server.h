#pragma once
#include "server.h"

// The queue of responces to client
typedef std::queue<std::pair<int, std::string>> Responses;

class TcpServer : public Server
{

private:
    // nothing

public:
    TcpServer(unsigned short port = TCP_PORT);
    ~TcpServer();
    int SetFDs(fd_set *reads, fd_set *writes, fd_set *exceptions);

    // Get fds after select() and proccess them
    // main service part
    bool GetFDs(fd_set *reads, fd_set *writes, fd_set *exceptions);

protected:
    Responses resp;
    std::vector<int> clients;

    bool Write(int soc, std::string response);
    std::string Read(int soc);
};