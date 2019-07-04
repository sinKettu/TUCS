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
    int udpSoc = -1; // do list of sockets for UDP
    struct sockaddr_in tcpSockAddr;
    struct sockaddr_in udpSockAddr;
    std::vector<int> connectedSockets;

public:
    bool Init();
    bool Serve();

protected:
    // nothing

};