#include <iostream>
#include "server.h"


int main()
{
    server s;
    s.add_signal(SIGINT, [&]()
    {
        s.stop();
    });
    s.run();
    return 0;
}