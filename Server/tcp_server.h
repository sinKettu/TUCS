#include "server.h"

class TcpServer : public Server
{

private:

public:
    TcpServer(unsigned short port = TCP_PORT);
    ~TcpServer();
    void SetFDs(fd_set *reads, fd_set *writes, fd_set *exceptions);
    void GetFDs(fd_set *reads, fd_set *writes, fd_set *exceptions);

protected:
    std::vector<int> clients;

};