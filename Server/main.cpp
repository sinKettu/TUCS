#include "tcp_server.h"
#include "udp_server.h"
#include <iostream>
#include <unistd.h>

int main()
{
    std::cout << "Hello!\n";
    TcpServer ts;
    std::cout << "TCP Server is up at port " << TCP_PORT << "!\n";
    UdpServer us;
    std::cout << "UDP Server is up at port " << UDP_PORT << "!\n";
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
                if (read <= 0)
                    continue;
                if (!strcmp(command, "exit\n"))
                    break;
            }
        }
    }

    return 0;
}