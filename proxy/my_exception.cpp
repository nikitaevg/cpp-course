//
// Created by Никита Воробьёв on 4/24/17.
//

#include "my_exception.h"

my_exception::my_exception(std::string msg) : msg(msg)
{}

std::string my_exception::get_msg() const
{
    return msg;
}
