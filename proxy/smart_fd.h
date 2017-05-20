#ifndef PROXY_SMART_FD_H
#define PROXY_SMART_FD_H


struct smart_fd
{
    explicit smart_fd(int fd);

    int close();

    operator int() const;

    smart_fd &operator=(const smart_fd &) = delete;

    smart_fd(const smart_fd &) = delete;

    void operator=(smart_fd &&);

    smart_fd(smart_fd &&);

    ~smart_fd();

    int operator*() const;

private:
    int fd;
};


#endif //PROXY_SMART_FD_H
