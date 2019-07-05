#include "server.h"
#include <unistd.h>
#include <algorithm>

Server::~Server()
{
    if (serverSocket > 0)
        close(serverSocket);
    
    Responses tmp;
    std::swap(resp, tmp);
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

bool Server::Write(int soc, std::string response)
{
    if (soc <= 0 || response.length() > 1024)
        return false;

    int res = write(soc, response.c_str(), response.length());
    return res == static_cast<int>(response.length());
}

std::string Server::Read(int soc)
{
    if (soc <= 0)
        return "";
    
    char buf[1024];
    int res = read(soc, buf, 1024);
    if (res > 0)
        return std::string(buf, res);
    else
        return "";
}

std::string Server::ProcessRequest(std::string req)
{
    char *ptr;
    unsigned long long sum = 0;
    std::vector<unsigned long long> toSort;
    while (true)
    {
        unsigned long long tmp = strtoull(req.c_str(), &ptr, 10);
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