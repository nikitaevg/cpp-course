#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <iosfwd>
#include <cstdint>
#include <vector>

struct big_integer;

static void add(big_integer& a, big_integer const& b);
static void my_invert(big_integer & r);
static size_t size(big_integer const& a);
static void div_int(big_integer& a, int v);
static void sub(big_integer& a, big_integer const& b);
static void binary_operation(big_integer& a, big_integer b, int64_t (*f)(int64_t, int64_t));
static int compare_abs(big_integer const& a, big_integer const& b);
static void mov_vect(big_integer& a);

struct big_integer
{
    big_integer();
    big_integer(big_integer const& other);
    big_integer(int a);
    explicit big_integer(std::string const& str);
    ~big_integer();

    big_integer& operator=(big_integer const& other);

    big_integer& operator+=(big_integer const& rhs);
    big_integer& operator-=(big_integer const& rhs);
    big_integer& operator*=(big_integer const& rhs);
    big_integer& operator/=(big_integer const& rhs);//
    big_integer& operator%=(big_integer const& rhs);//

    big_integer& operator&=(big_integer const& rhs);
    big_integer& operator|=(big_integer const& rhs);
    big_integer& operator^=(big_integer const& rhs);

    big_integer& operator<<=(int rhs);
    big_integer& operator>>=(int rhs);

    big_integer operator+() const;
    big_integer operator-() const;
    big_integer operator~() const;

    big_integer& operator++();
    big_integer operator++(int);


    big_integer& operator--();
    big_integer operator--(int);

    friend bool operator==(big_integer const& a, big_integer const& b);
    friend bool operator!=(big_integer const& a, big_integer const& b);
    friend bool operator<(big_integer const& a, big_integer const& b);
    friend bool operator>(big_integer const& a, big_integer const& b);
    friend bool operator<=(big_integer const& a, big_integer const& b);
    friend bool operator>=(big_integer const& a, big_integer const& b);
    
    friend std::string to_string(big_integer const& a);

private:
    bool pos;
    std::vector<int64_t> data;
    friend void add(big_integer& a, big_integer const& b);
    friend void my_invert(big_integer & r);
    friend size_t size(big_integer const& a);
    friend void div_int(big_integer& a, int v);
    friend void sub(big_integer& a, big_integer const& b);
    friend void binary_operation(big_integer& a, big_integer b, int64_t (*f)(int64_t, int64_t));
    friend int compare_abs(big_integer const& a, big_integer const& b);
    friend void mov_vect(big_integer& a);
};

big_integer operator-(big_integer a, big_integer const& b);
big_integer operator+(big_integer a, big_integer const& b);
big_integer operator*(big_integer a, big_integer const& b);
big_integer operator/(big_integer a, big_integer const& b);
big_integer operator%(big_integer a, big_integer const& b);

big_integer operator&(big_integer a, big_integer const& b);
big_integer operator|(big_integer a, big_integer const& b);
big_integer operator^(big_integer a, big_integer const& b);

big_integer operator<<(big_integer a, int b);
big_integer operator>>(big_integer a, int b);

bool operator==(big_integer const& a, big_integer const& b);
bool operator!=(big_integer const& a, big_integer const& b);
bool operator<(big_integer const& a, big_integer const& b);
bool operator>(big_integer const& a, big_integer const& b);
bool operator<=(big_integer const& a, big_integer const& b);
bool operator>=(big_integer const& a, big_integer const& b);

std::string to_string(big_integer const& a);
std::ostream& operator<<(std::ostream& s, big_integer const& a);

#endif // BIG_INTEGER_H
