#include "socket.h"
#include "my_exception.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>

namespace
{
    int make_socket(int domain, int type, int protocol)
    {
        int fd = socket(domain, type, protocol);
        error(fd, "Make", -1);
        return fd;
    }

    void bind_socket(int fd, sockaddr* addr)
    {
        int res = bind(fd, addr, sizeof(*addr));
        error(fd, "bind", -1);
    }

    void listen_socket(int sock)
    {
        int fd = listen(sock, SOMAXCONN);
        error(fd, "listen", -1);
    }
}

int accept_socket(int sock, sockaddr* addr)
{
    socklen_t len = sizeof(*addr);
    int fd;
    while(true)
    {
        fd = accept(sock, addr, &len);
        if (fd == -1 && errno == ECONNABORTED)
            continue;
        error(fd, "Error in accepting " + std::to_string(errno), -1);
        break;
    }
    error(fd, "accept", -1);
    return fd;
}

int create_socket(sockaddr* sockad)
{
    int sock = make_socket(AF_INET, SOCK_STREAM, 0);
    bind_socket(sock, sockad);
    listen_socket(sock);
    return sock;
}

void error(int to_check, std::string msg, int err_val)
{
    if (to_check != err_val)
        return;
    throw my_exception(msg);

}