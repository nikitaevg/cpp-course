#ifndef PROXY_SERVER_H
#define PROXY_SERVER_H


#include "my_kqueue.h"

struct server
{
    void run();
    void stop();
    void add_signal(int, std::function<void()>);
private:
    static const uint32_t addr = INADDR_ANY;
    my_kqueues::my_kqueue kqueue;
    my_kqueues::my_kqueue::socket serv_sock;
};


#endif //PROXY_SERVER_H
