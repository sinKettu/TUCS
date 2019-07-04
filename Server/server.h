#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <vector>

#define TCP_PORT 9999
#define UDP_PORT 9998

class Server
{

private:
    int tcpSoc = -1;
    int udpSoc = -1;
    struct sockaddr_in tcpSockAddr;
    struct sockaddr_in udpSockAddr;

public:
    bool Init();

protected:
    // nothing

};