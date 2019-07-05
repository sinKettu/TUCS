#include "client.h"
#include <iostream>

Client::Client(char type)
{
    if (type == TYPE_TCP || type == TYPE_UDP)
        protocol = type;
    else
    {
        std::cout << "Unknown protocol\n";
        exit(1);
    }
    memset(&sa, 0, sizeof(sa));
}

Client::~Client()
{
    if (soc > 0)
        close(soc);
    memset(&sa, 0, sizeof(sa));
}

bool Client::IsIP(std::string address)
{
    char *ptr;
    unsigned long tmp = strtoul(address.c_str(), &ptr, 10);
    if ((tmp == 0 && errno != 0) || tmp > 255)
        return false;
    
    for (char i = 0; i < 3; i++)
    {
        tmp = strtoul(ptr + 1, &ptr, 10);
        if ((tmp == 0 && errno != 0) || tmp > 255)
            return false;
    }

    if (ptr - address.c_str() != address.length())
        return false;
    else
        return true;
}

void Client::ResolveHostName(in_addr *ia, char *hostname)
{
    hostent *h = gethostbyname(hostname);
    if (h == NULL)
    {
        ia = nullptr;
        return;
    }

    in_addr *tmp = ia;
    for (int i = 0; h->h_addr_list[i]; i++)
    {
        memcpy(tmp, h->h_addr_list[i], h->h_length);
        tmp += h->h_length;
        //debug
        char *a = inet_ntoa(*ia);
        a = inet_ntoa(*tmp);
    }
}

bool Client::Connect(std::string address, unsigned short port)
{
    if (protocol == TYPE_TCP)
    {
        soc = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (soc == -1)
        {
            std::cout << "Couldn't create TCP socket\n";
            close(soc);
            return false;
        }
    }
    else if (protocol == TYPE_UDP)
    {
        soc = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (soc == -1)
        {
            std::cout << "Couldn't create UDP socket\n";
            close(soc);
            return false;
        }
    }
    else
    {
        std::cout << "Unknow protocol\n";
        return false;
    }

    sa.sin_family = PF_INET;
    sa.sin_port = htons(port);
    if (IsIP(address))
        sa.sin_addr.s_addr = inet_addr(address.c_str());
    else
    {
        ResolveHostName(&sa.sin_addr, const_cast<char*>(address.c_str()));
        if (&sa.sin_addr == nullptr)
        {
            std::cout << "Given address is not valid\n";
            close(soc);
            return false;
        }
    }

    if (protocol == TYPE_TCP && connect(soc, (struct sockaddr*)&sa, sizeof(sa)) == -1)
    {
        std::cout << "Couldn't establish TCP connection\n";
        close(soc);
        return false;
    }

    return true;
}

bool Client::Send(std::string message)
{
    if (message.length() > 1024)
    {
        std::cout << "Message length is too big\n";
        return false;
    }
    
    int res = sendto(soc, message.c_str(), message.length(), 0, (struct sockaddr*)&sa, sizeof(sa));
    return res == message.length();
}

std::string Client::Receive()
{
    char buf[1024];
    socklen_t sl = sizeof(sa);
    int l = recvfrom(soc, buf, 1024, 0, (struct sockaddr*)&sa, &sl);
    if (l <= 0)
        return "";
    else
        return std::string(buf, l);
}