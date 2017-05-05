#ifndef PROXY_MY_KQUEUE_H
#define PROXY_MY_KQUEUE_H


#include <sys/event.h>
#include <array>
#include <vector>
#include <map>
#include "smart_fd.h"
#include <functional>

struct my_kqueue
{
private:
    using handler_t = std::function<void()> ;
public:
    struct socket
    {
    private:
        static constexpr size_t BUFF_SIZE = 1024;
    public:
        socket();
        socket(smart_fd&&);
        socket& operator=(const socket&) = delete;
        socket(const socket&) = delete;
        void operator=(socket&&);
        smart_fd& get_fd();
        void set_on_read(handler_t&&);
        void set_on_write(handler_t&&);
        void set_on_close(handler_t&&);
        struct metadata
        {
            std::array<char, BUFF_SIZE> buff;
            ssize_t red;
        } m_data;
        metadata& get_meta_data();
    private:
        smart_fd fd;
        size_t time;
        handler_t on_read, on_write, on_close;
        static constexpr size_t DEFAULT_TIME = 5000;
        std::array<char, BUFF_SIZE> buffer;
        friend class my_kqueue;
    };

    constexpr static int MAX_NUMBER_OF_CLIENTS = 1024;
    my_kqueue();
    size_t add_socket(socket &&, bool to_read, bool once);
    socket & get_socket(size_t);
    void remove_socket(size_t, bool to_read);
    void change_aim(size_t, bool);
    void stop();
    void add_signal(int, handler_t&&);
    void listen();
private:
    bool working = true;
    std::array<struct kevent, MAX_NUMBER_OF_CLIENTS> triggered;
    size_t num_of_clients = 0;
    smart_fd kqu;
    std::map<int, socket> fd_to_socket;
    std::map<int, handler_t> sig_to_handler;
    size_t common_add(socket &&, int, bool);
    ssize_t run_query(int, int, int, int = 0, int = 0, void * = nullptr);
};


#endif //PROXY_MY_KQUEUE_H
