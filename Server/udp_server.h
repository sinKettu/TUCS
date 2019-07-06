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

    // There is no need to use queue as in the case with TCP
    // So, reading (receiving), processing, writing (sending)
    // occur sequentially
    void ReadProcessWrite(int soc);

};