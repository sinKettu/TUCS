#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <queue>
#include <utility>

#define TCP_PORT 9999
#define UDP_PORT 9998

// Parent class

class Server
{

private:
    // nothing

public:
    
    // Setting server socket fd to fd_set's for select()
    int SetFDs(fd_set *reads, fd_set *writes, fd_set *exceptions);
    ~Server();

protected:
    int serverSocket = -1;
    struct sockaddr_in serverSocAddr;

    // Processing client's requests
    // Extracting numbers, sorting, summation
    std::string ProcessRequest(std::string req);

};