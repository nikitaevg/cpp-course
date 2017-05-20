//
// Created by Никита Воробьёв on 5/14/17.
//

#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include "server.h"
#include "smart_fd.h"
#include "socket.h"
#include "my_kqueue.h"

void server::run()
{
    sockaddr_in sin = make_sockaddr(AF_INET, addr, 8001);
    sockaddr *sockad = reinterpret_cast<sockaddr *>(&sin);
    smart_fd sock(create_socket(sockad));

    using socket = my_kqueues::my_kqueue::socket;
    size_t server_sd;
    serv_sock = socket(std::move(sock));
    serv_sock.set_on_read(
            [&]()
            {
                socket client(smart_fd(accept_socket(kqueue.get_socket(server_sd).get_fd(), sockad)));
                fcntl(client.get_fd(), F_SETFD, FD_CLOEXEC);
                size_t client_sd;
                client_sd = kqueue.add_socket(std::move(client), true, false, false);
                socket &client_sock = kqueue.get_socket(client_sd);
                socket::metadata &m_data = client_sock.get_meta_data();
                client_sock.set_on_read(
                        [&, client_sd]()
                        {
                            m_data.red = read(client_sock.get_fd(), m_data.buff.data(),
                                              m_data.buff.size());
                            if (!m_data.red)
                            {
                                kqueue.remove_socket(client_sd);
                                return;
                            }
                            kqueue.change_aim(client_sd, true);
                        });
                client_sock.set_on_write(
                        [&, client_sd]()
                        {
                            ssize_t a = write(client_sock.get_fd(), m_data.buff.data() + m_data.written,
                                              static_cast<size_t>(m_data.red - m_data.written));
                            if (a != -1)
                                m_data.written += a;
                            if (m_data.written == m_data.red)
                            {
                                kqueue.change_aim(client_sd, false);
                                m_data.inval();
                            }
                        });
            });
    server_sd = kqueue.add_socket(std::move(serv_sock), true, false, true);
    kqueue.listen();
}

void server::stop()
{
    kqueue.stop();
}

void server::add_signal(int signal, std::function<void()> handler)
{
    kqueue.add_signal(signal, std::move(handler));
}
