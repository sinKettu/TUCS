#pragma once
#include "server.h"

class UdpServer : public Server
{

private:
    // nothing

public:
    UdpServer(unsigned short port = UDP_PORT);
    bool GetFDs(fd_set *reads, fd_set *writes, fd_set *exceptions);

protected:
    // nothing

};