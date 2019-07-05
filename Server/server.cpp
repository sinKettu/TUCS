#include "server.h"
#include <unistd.h>
#include <algorithm>

Server::~Server()
{
    if (serverSocket > 0)
        close(serverSocket);
    
    memset(&serverSocAddr, 0, sizeof(serverSocAddr));
}

int Server::SetFDs(fd_set *reads, fd_set *writes, fd_set *exceptions)
{
    if (serverSocket <= 0)
        return -1;
    
    FD_SET(serverSocket, reads);
    FD_SET(serverSocket, writes);
    FD_SET(serverSocket, exceptions);

    return serverSocket;
}

std::string Server::ProcessRequest(std::string req)
{
    char *str = const_cast<char*>(req.c_str());
    char *ptr = str;
    unsigned long long sum = 0;
    std::vector<unsigned long long> toSort;
    while (true)
    {
        while (!(*str >= '0' && *str <= '9') && *str) str++;
        if (!(*str))
            break;
        
        unsigned long long tmp = strtoull(str, &ptr, 10);
        str = ptr;
        if (tmp == 0 && errno != 0)
            break;
        
        if (UINT64_MAX - sum < tmp || errno == ERANGE)
            return "Overflow error\n";
        
        // На данном моменте можно обойтись без сортировки
        // Если применить двоичный поиск со вставкой, то сразу
        // после парсинга можно получить отсортированный в нужном
        // порядке вектор. Это займет меньше действий и израсходует
        // меньше памяти. Таким свойством также обладает
        // класс std::map, но его использование займет больше ресурсов
        sum += tmp;
        toSort.push_back(tmp);
    }

    if (toSort.empty())
    {
        return "There was nothing to process\n";
    }
    else
    {
        std::sort(toSort.begin(), toSort.end());
        std::string result = "";
        std::vector<unsigned long long>::iterator num;
        for (num = toSort.begin(); num != toSort.end(); num++)
        {
            result.append(std::to_string(*num));
            result.append(" ", 1);
        }
        result.pop_back();
        result.push_back('\n');
        result.append(std::to_string(sum));
        result.push_back('\n');
        result.push_back(0);

        return result;
    }
}