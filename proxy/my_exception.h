//
// Created by Никита Воробьёв on 4/24/17.
//

#ifndef PROXY_MY_EXCEPTION_H
#define PROXY_MY_EXCEPTION_H


#include <exception>
#include <string>

struct my_exception : std::exception
{
    my_exception(std::string msg);

    std::string get_msg() const;

private:
    std::string msg;
};


#endif //PROXY_MY_EXCEPTION_H
