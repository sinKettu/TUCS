#include "tcp_server.h"
#include "udp_server.h"
#include <iostream>
#include <unistd.h>

bool PortsRequest(unsigned short &tp, unsigned short &up)
{
    std::cout << "Please, enter TCP and UDP ports or press <ENTER>\n";
    std::cout << "to use default ones (TCP: " << TCP_PORT << ", UDP: " << UDP_PORT << ")\n";
    std::cout << "TCP: ";
    std::string tmp;
    std::getline(std::cin, tmp);
    if (tmp.empty())
    {
        tp = TCP_PORT;
    }
    else
    {
        char *ptr;
        unsigned long n = strtoul(tmp.c_str(), &ptr, 10);
        if (errno != 0 || n == 0 || n > UINT16_MAX || ptr - tmp.c_str() != tmp.length())
        {
            tp = up = 0;
            return false;
        }
        else
        {
            tp = static_cast<unsigned short>(n);
        }
    }

    std::cout << "UDP: ";
    std::getline(std::cin, tmp);
    if (tmp.empty())
    {
        up = UDP_PORT;
    }
    else
    {
        char *ptr;
        unsigned long n = strtoul(tmp.c_str(), &ptr, 10);
        if (errno != 0 || n == 0 || n > UINT16_MAX || ptr - tmp.c_str() != tmp.length())
        {
            tp = up = 0;
            return false;
        }
        else
        {
            up = static_cast<unsigned short>(n);
        }
    }

    return true;
}

int main()
{
    std::cout << "Hello!\n";
    unsigned short tp, up;
    if (!PortsRequest(tp, up))
    {
        std::cout << "Wrong port!\n";
        return 1;
    }
    TcpServer ts(tp);
    std::cout << "TCP Server is up at port " << tp << "!\n";
    UdpServer us(up);
    std::cout << "UDP Server is up at port " << up << "!\n";
    std::cout << "Type 'exit' to close program\n";
    fd_set rfds, wfds, efds;
    while (true)
    {
        FD_ZERO(&rfds);
        FD_ZERO(&wfds);
        FD_ZERO(&efds);
        FD_SET(STDIN_FILENO, &rfds);

        int tn = ts.SetFDs(&rfds, &wfds, &efds);
        if (tn < 0)
            return 1;

        int un = us.SetFDs(&rfds, &wfds, &efds);
        if (un < 0)
            return 1;

        timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 500;
        int res = select(std::max(tn, un) + 1, &rfds, &wfds, &efds, &tv);
        if (res < 0)
        {
             std::cout << "Unknow error!\n";
             return 1;
        }
        else if (res == 0)
        {
           continue;
        }
        else
        {
            if (!(us.GetFDs(&rfds, &wfds, &efds) && ts.GetFDs(&rfds, &wfds, &efds)))
                return 1;
            
            if (FD_ISSET(STDIN_FILENO, &rfds))
            {
                char command[6]; // exit\n\0
                int len = read(STDIN_FILENO, command, 6);
                if (len <= 0)
                    continue;
                if (!strncmp(command, "exit\n", 5))
                    break;
            }
        }
    }

    return 0;
}