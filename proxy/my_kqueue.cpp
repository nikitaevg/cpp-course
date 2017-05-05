#include <iostream>
#include <csignal>
#include "my_kqueue.h"
#include "my_exception.h"

my_kqueue::my_kqueue() :
        kqu(kqueue())
{
}

int bool_to_read_write(bool to_read)
{
    return (to_read ? EVFILT_READ : EVFILT_WRITE);
}

void my_kqueue::listen()
{
    while (working)
    {
        int sz = kevent(kqu, nullptr, 0, triggered.data(), MAX_NUMBER_OF_CLIENTS, NULL);
        if (sz == -1)
            throw my_exception("Couldn't listen to kqueue");
        for (int i = 0; i < sz && working; i++)
        {
            intptr_t fd = triggered[i].ident;
            if (triggered[i].filter == EVFILT_WRITE)
                fd_to_socket[fd].on_write();
            else if (triggered[i].filter == EVFILT_READ)
                fd_to_socket[fd].on_read();
            else if (triggered[i].filter == EVFILT_SIGNAL)
                sig_to_handler[fd]();
        }
    }
}


size_t my_kqueue::common_add(socket &&sock, int read_write, bool one_shot)
{
    run_query(sock.fd, read_write, EV_ADD | (one_shot ? EV_ONESHOT : 0));
    size_t res = static_cast<size_t>(sock.fd);
    fd_to_socket[sock.fd] = std::move(sock);
    return res;
}

void my_kqueue::change_aim(size_t sd, bool was_to_read)
{
    socket &s = fd_to_socket[sd];
    int prev = EVFILT_READ, curr = EVFILT_WRITE;
    if (!was_to_read)
        std::swap(prev, curr);
    run_query(s.get_fd(), prev, EV_DELETE);
    run_query(s.get_fd(), curr, EV_ADD);
}

void my_kqueue::remove_socket(size_t sd, bool to_read)
{
    socket &socket_fd = fd_to_socket[sd];
    run_query(socket_fd.get_fd(), bool_to_read_write(to_read), EV_DELETE);
    fd_to_socket.erase(static_cast<int>(sd));
}

size_t my_kqueue::add_socket(socket &&sock, bool to_read, bool once)
{
    return common_add(std::move(sock), bool_to_read_write(to_read), once);
}

my_kqueue::socket &my_kqueue::get_socket(size_t sd)
{
    return fd_to_socket[sd];
}

ssize_t my_kqueue::run_query(int a, int b, int c, int d, int e, void *f)
{
    struct kevent ev;
    EV_SET(&ev, a, b, c, d, e, f);
    return kevent(kqu, &ev, 1, nullptr, 0, nullptr);
}

void my_kqueue::add_signal(int signal_fd, handler_t &&handler)
{
    std::signal(signal_fd, SIG_IGN);
    run_query(signal_fd, EVFILT_SIGNAL, EV_ADD);
    sig_to_handler[signal_fd] = std::move(handler);
}

void my_kqueue::stop()
{
    for (auto& t : fd_to_socket)
    {
        remove_socket(static_cast<size_t>(t.first), true);
        remove_socket(static_cast<size_t>(t.first), false); // BAD CODE
    }
    working = false;
}

void my_kqueue::socket::set_on_read(my_kqueue::handler_t &&on_read)
{
    this->on_read = on_read;
}

void my_kqueue::socket::set_on_write(my_kqueue::handler_t &&on_write)
{
    this->on_write = on_write;
}

void my_kqueue::socket::set_on_close(my_kqueue::handler_t &&on_close)
{
    this->on_close = on_close;
}

my_kqueue::socket::socket(smart_fd &&fd) : fd(std::move(fd)), time(DEFAULT_TIME)
{
}

my_kqueue::socket::socket() : socket(-1)
{
}

my_kqueue::socket::metadata &my_kqueue::socket::get_meta_data()
{
    return m_data;
}

smart_fd &my_kqueue::socket::get_fd()
{
    return fd;
}

void my_kqueue::socket::operator=(my_kqueue::socket &&rhs)
{
    this->fd = std::move(rhs.fd);
    this->on_write = rhs.on_write;
    this->on_read = rhs.on_read;
    this->on_close = rhs.on_close;
    this->time = rhs.time;
}
