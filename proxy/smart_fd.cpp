#include <unistd.h>
#include <assert.h>
#include <iostream>
#include "smart_fd.h"

smart_fd::smart_fd(int fd) : fd(fd)
{
}

int smart_fd::close()
{
    if (fd == -1)
        return 0;
    int fd = ::close(this->fd);
    assert(fd == 0);
    return fd;
}

int smart_fd::operator*() const
{
    return fd;
}

smart_fd::operator int() const
{
    return fd;
}

smart_fd::~smart_fd() noexcept
{
    this->close();
}

smart_fd::smart_fd(smart_fd &&rhs)
{
    this->fd = rhs.fd;
    rhs.fd = -1;
}

void smart_fd::operator=(smart_fd &&rhs)
{
    this->fd = rhs.fd;
    rhs.fd = -1;
}
