#ifndef PROXY_SOCKET_H
#define PROXY_SOCKET_H

#include <sys/socket.h>
#include <string>
#include <netinet/in.h>

int create_socket(sockaddr *sockad);

int accept_socket(int sock, sockaddr *addr);

sockaddr_in make_sockaddr(int family, uint32_t addr, uint16_t port);

void error(int to_check, std::string msg, int err_val);


#endif //PROXY_SOCKET_H
