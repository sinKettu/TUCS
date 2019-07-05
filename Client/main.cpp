#include "client.h"
#include <string>
#include <iostream>

void Usage()
{
    std::cout << "Usage: ./test_client [options]\n";
    std::cout << "'-t' -- TCP connection, required flag.\n";
    std::cout << "'-u' -- UDP connection, required flag.\n";
    std::cout << "One of two must be selected.\n";
    std::cout << "'-a address:port' -- specifying target, optional flag\n";
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        Usage();
        return 1;
    }

    // parsing given arguments
    int i = 1;
    char type = -1;
    std::string address;
    unsigned long port = 0;
    while (i < argc)
    {
        if (!strcmp(argv[i], "-u") && type == -1)
        {
            type = TYPE_UDP;
            i++;
        }
        else if (!strcmp(argv[i], "-t") && type == -1)
        {
            type = TYPE_TCP;
            i++;
        }
        else if (!strcmp(argv[i], "-a") && port == 0)
        {
            if (++i >= argc)
            {
                Usage();
                return 1;
            }

            int pos = 0;
            for (; !argv[i][pos] || argv[i][pos] == ':'; pos++);
            if (argv[i][pos])
            {
                address = std::string(argv[i], pos);
                port = strtoul(argv[i] + pos + 1, NULL, 10);
                if (port == 0 || port > UINT16_MAX || errno != 0)
                {
                    Usage();
                    return 1;
                }
                i++;
            }
            else
            {
                Usage();
                return 1;
            }
        }
        else
        {
            Usage();
            return 1;
        }
    }

    if (type == -1)
    {
        Usage();
        return 1;
    }
    std::cout << "Hello!\n";
    std::cout << "Use CTRL+C to close program\n";
    if (port == 0)
    {
        std::cout << "Seems like you didn't specify target\n";
        std::cout << "Address: ";
        std::getline(std::cin, address);
        std::cout << "Port: ";
        std::cin >> port;
        if (port == 0 || port > UINT16_MAX)
        {
            std::cout << "Wrong port\n";
            return 1;
        }
    }

    Client client(type);
    if (!client.Connect(address, static_cast<unsigned short>(port)))
    {
        std::cout << "Connection failed!\n";
        return 1;
    }

    while (true)
    {
        std::string request = "";
        std::cout << ">> ";
        std::getline(std::cin, request);
        client.Send(request);
        std::string response = client.Receive();
        std::cout << "Response: " << response << "\n";
    }
}