#include <algorithm>
#include "timer.h"

using timers::timer;

timer::timer(size_t sd, time_t limit) :
        sd(sd),
        last(time(0)),
        limit(limit)
{}

timer::timer(const timer &rhs) :
        sd(rhs.sd),
        last(time(0)),
        limit(rhs.limit)
{

}

void timer::update_time()
{
    last = time(0);
}

time_t timer::left() const
{
    if (limit - (time(0) - last) < 0)
        return 0;
    return limit - (time(0) - last);
}

bool timer::operator<(const timer &rhs) const
{
    if (last == rhs.last)
        return get_sd() < rhs.get_sd();
    return left() < rhs.left();
}

size_t timer::get_sd() const
{
    return sd;
}

timer::timer() : limit(0) // needed bu map. although seems like shit
{
}
