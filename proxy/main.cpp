#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include "socket.h"
#include "smart_fd.h"
#include "my_kqueue.h"
#include <stdio.h>
#include <string.h>
#include <string>
#include <fcntl.h>
#include <signal.h>
#include <sys/signal.h>
#include <csignal>

const uint32_t addr = INADDR_ANY;

sockaddr_in make_sockaddr(int family, uint32_t addr, uint16_t port)
{
    sockaddr_in sa;
    sa.sin_family = family;
    sa.sin_port = port;
    sa.sin_addr.s_addr = addr;

    return sa;
}

int main()
{
    sockaddr_in sin = make_sockaddr(AF_INET, addr, 8001);
    sockaddr *sockad = reinterpret_cast<sockaddr *>(&sin);
    smart_fd sock(create_socket(sockad));

    using socket = my_kqueue::socket;
    my_kqueue kqueue;
    size_t server_sd;

    socket serv_sock(std::move(sock));
    serv_sock.set_on_read([&]()
      {
          socket client(accept_socket(kqueue.get_socket(server_sd).get_fd(), sockad));
          fcntl(client.get_fd(), F_SETFD, FD_CLOEXEC);
          size_t client_sd;
          client_sd = kqueue.add_socket(std::move(client), true, false);
          socket& client_sock = kqueue.get_socket(client_sd);
          socket::metadata& m_data = client_sock.get_meta_data();
          client_sock.set_on_read([&, client_sd]()
             {
                 m_data.red = read(client_sock.get_fd(), m_data.buff.data(), m_data.buff.size());
                 if (!m_data.red)
                 {
                     kqueue.remove_socket(client_sd, true);
                     std::cout << "Closed\n";
                     return;
                 }
                 kqueue.change_aim(client_sd, true);
             });
          client_sock.set_on_write([&, client_sd]()
              {
                  write(client_sock.get_fd(), m_data.buff.data(), m_data.red);
                  kqueue.change_aim(client_sd, false);
              });
      });
    server_sd = kqueue.add_socket(std::move(serv_sock), true, false);
    kqueue.add_signal(SIGINT, [&]()
    {
        kqueue.stop();
    });
    kqueue.listen();
    return 0;
}