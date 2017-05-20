#ifndef PROXY_TIMER_H
#define PROXY_TIMER_H


#include <cstddef>
#include <ctime>

namespace timers
{
    struct timer
    {

        timer();

        explicit timer(size_t, time_t limit);

        timer(const timer &);

        void update_time();

        time_t left() const;

        bool operator<(const timer &) const;

        size_t get_sd() const;

    private:
        time_t last;
        size_t sd;
        const time_t limit;
    };
}


#endif //PROXY_TIMER_H
