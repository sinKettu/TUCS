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
    int soc = -1;
    sockaddr_in sa;
    char protocol = -1;

    bool IsIP(std::string address);
    void ResolveHostName(in_addr *ia, char *hostname);

public:
    Client(char type);
    ~Client();
    bool Connect(std::string address, unsigned short port);
    bool Send(std::string message);
    std::string Receive();

};