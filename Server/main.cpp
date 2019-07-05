#include "tcp_server.h"
#include "udp_server.h"
#include <iostream>
#include <unistd.h>

int main()
{
    std::cout << "Hello!\n";
    TcpServer ts;
    //UdpServer us;
    fd_set rfds, wfds, efds;
    while (true)
    {
        FD_SET(STDIN_FILENO, &rfds);
        int tn = ts.SetFDs(&rfds, &wfds, &efds);
        if (tn < 0)
            break;

        // int un = us.SetFDs(&rfds, &wfds, &efds);
        // if (un < 0)
        //    break;

        timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 500;
        // int res = select(std::max(tn, un), &rfds, &wfds, &efds, &tv);
        // if (res < 0)
        // {
        //      std::cout << "Unknow error!\n";
        //      break;
        // }
        // else if (res == 0)
        // {
        //    continue;
        // }
    }

    return 0;
}