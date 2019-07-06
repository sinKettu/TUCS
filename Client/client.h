#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <unistd.h>

#define TYPE_TCP 0
#define TYPE_UDP 1

class Client
{

private:
    // socket for connections
    int soc = -1;
    sockaddr_in sa;

    // used protocol: TCP / UDP
    char protocol = -1;

    // Check whether string has IP format: ddd.ddd.ddd.ddd
    bool IsIP(std::string address);

    // Resolving hostname (URL) to IP
    void ResolveHostName(in_addr *ia, char *hostname);

public:
    Client(char type);
    ~Client();

    bool Connect(std::string address, unsigned short port);
    bool Send(std::string message);
    std::string Receive();

};