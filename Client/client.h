#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <unistd.h>

class Client
{

private:
    int socket;
    sockaddr sa;

    Client();
    ~Client();

public:
    bool Connect(std::string address, unsigned int port);
    bool Send(std::string message);
    std::string Receive();

};