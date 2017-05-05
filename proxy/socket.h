#ifndef PROXY_SOCKET_H
#define PROXY_SOCKET_H

#include <sys/socket.h>
#include <string>

int create_socket(sockaddr* sockad);
int accept_socket(int sock, sockaddr* addr);
void error(int to_check, std::string msg, int err_val);


#endif //PROXY_SOCKET_H
