#include <iostream>
#include <sstream>
#include "big_integer.h"
#include <vector>

#define int64_t int_fast64_t
#define int32_t int_fast32_t

const int START_SIZE = 4;
const long long BASE = ((int64_t) 1 << (int64_t) 31);
const int32_t LOG_BASE = 31;
int cnt = 0;

static inline int64_t get_bool_vect(std::vector<int64_t> a, size_t i, bool q) {
    return (i < a.size() ? a[i] : q);
}

static inline void delete_zeroes(std::vector<int64_t> &a) {
    while (a.size() > 0 && !a.back())
        a.pop_back();
}

static inline void check_vector(std::vector<int64_t> &a, uint32_t require) {
    while (a.size() <= require) {
        a.push_back(0);
    }
}

static inline void add(big_integer &a, big_integer const &b) {
    int64_t carry = 0;
    for (size_t i = 0; i < size(b) || carry; i++) {
        check_vector(a.data, i);
        a.data[i] = (a.data[i] + (i < size(b) ? b.data[i] : 0) + carry);
        carry = 0;
        if (a.data[i] >= BASE) {
            a.data[i] -= BASE;
            carry = 1;
        }
    }
}

static inline void sub(big_integer &a, big_integer const &b) {
    int64_t carry = 0;
    for (size_t i = 0; i < size(b) || carry; i++) {
        check_vector(a.data, i);
        a.data[i] = a.data[i] - (i < size(b) ? b.data[i] : 0) - carry;
        carry = 0;
        if (a.data[i] < 0) {
            a.data[i] += BASE;
            carry = 1;
        }
    }
}

big_integer::big_integer() : 
    data(std::vector<int64_t>()){}

big_integer::big_integer(big_integer const &other) :
    pos(other.pos),
    data(other.data) {}

big_integer::big_integer(int x) {
    int64_t a = x;
    this->pos = true;
    if (a < 0) {
        this->pos = false;
        a = -a;
    }
    while (a) {
        data.push_back(a % BASE);
        a /= BASE;
    }
}

big_integer::big_integer(std::string const &str) {
    bool q = true;
    std::string s = str;
    this->pos = true;
    if (*s.begin() == '-') {
        q = false;
    }
    big_integer power = 1;
    *this = 0;
    for (int32_t i = str.size() - 1; i >= !q; i--) {
        *this += (str[i] - '0') * power;
        power *= 10;
    }
    this->pos = q;
}

big_integer::~big_integer() {

}

static int compare_abs(big_integer const &a, big_integer const &b) {
    big_integer x = a;
    big_integer y = b;
    x.pos = 1;
    y.pos = 1;
    if (x < y)
        return -1;
    else if (x == y)
        return 0;
    else
        return 1;
}

static inline size_t size(big_integer const &a) {
    return a.data.size();
}

std::string to_string(big_integer const &a) {
    std::string ret = "";
    big_integer ten = 10;
    big_integer t = a, temp;
    t.pos = true;
    bool q = false;
    while (t != 0) {
        temp = (t % 10);
        ret = ret + (char) (temp.data[0] + '0');
        t /= 10;
    }
    std::reverse(ret.begin(), ret.end());
    if (!a.pos && ret != "")
        ret = "-" + ret;
    if (ret == "")
        ret = "0";
    return ret;
}

std::ostream &operator<<(std::ostream &s, big_integer const &a) {
    return s << to_string(a);
}

big_integer &big_integer::operator=(big_integer const &other) {
    pos = other.pos;
    data = std::vector<int64_t>(other.data);
    return *this;
}

big_integer &big_integer::operator+=(big_integer const &rhs) {
    data.resize(std::max(data.size(), size(rhs)));
    if (this->pos == rhs.pos) {
        add(*this, rhs);
    }
    else if (compare_abs(*this, rhs) >= 0) {
        sub(*this, rhs);
    }
    else {
        big_integer temp = rhs;
        sub(temp, *this);
        *this = temp;
    }
    return *this;
}

big_integer &big_integer::operator-=(big_integer const &rhs) {
    return *this += (-rhs);
}

big_integer &big_integer::operator++() {
    *this += 1;
    return *this;
}

big_integer big_integer::operator++(int) {
    big_integer ret = *this;
    ++*this;
    return ret;
}

big_integer &big_integer::operator--() {
    *this -= 1;
    return *this;
}

big_integer big_integer::operator--(int) {
    big_integer ret = *this;
    --*this;
    return ret;
}

bool operator==(big_integer const &a, big_integer const &b) {
    size_t sz = std::max(size(a), size(b));
    bool q = false;
    for (size_t i = 0; i < sz; i++) {
        if ((i >= size(a) && b.data[i]) || (i >= size(b) && a.data[i]) ||
            (i < size(a) && i < size(b) && a.data[i] != b.data[i]))
            return false;
        q |= get_bool_vect(a.data, i, 0) | get_bool_vect(b.data, i, 0);
    }
    if (q && a.pos != b.pos)
        return false;
    return true;
}

bool operator!=(big_integer const &a, big_integer const &b) {
    return !(a == b);
}

bool operator<(big_integer const &a, big_integer const &b) {
    int32_t sz = std::max(size(a), size(b));
    if (a == b)
        return false;
    if (a.pos != b.pos) {
        return !a.pos;
    }
    for (int32_t i = sz - 1; i >= 0; i--) {
        if ((i >= size(b) && a.data[i]) || (i < size(a) && i < size(b) && a.data[i] > b.data[i]))
            return false;
        else if ((i >= size(a) && b.data[i]) || (i < size(a) && i < size(b) && a.data[i] < b.data[i]))
            return true;
    }
    return false;
}

bool operator>(big_integer const &a, big_integer const &b) {
    return b < a;
}

bool operator<=(big_integer const &a, big_integer const &b) {
    return (a == b || a < b);
}

bool operator>=(big_integer const &a, big_integer const &b) {
    return b <= a;
}

big_integer big_integer::operator-() const {
    big_integer ret = *this;
    ret.pos = !ret.pos;
    return ret;
}

big_integer big_integer::operator+() const {
    big_integer ret = *this;
    return ret;
}

big_integer operator-(big_integer a, big_integer const &b) {
    return a -= b;
}

big_integer operator+(big_integer a, big_integer const &b) {
    return a += b;
}

big_integer &big_integer::operator*=(big_integer const &rhs) {
    big_integer ans;
    ans.data.resize(size(*this) + size(rhs));
    int64_t t, carry;
    for (size_t i = 0; i < size(*this); i++) {
        carry = 0;
        for (size_t j = 0; j < size(rhs) || carry; j++) {
            t = ans.data[i + j] + data[i] * (j < size(rhs) ? rhs.data[j] : 0) + carry;
            ans.data[i + j] = (t % BASE);
            carry = t / BASE;
        }
    }
    ans.pos = (pos == rhs.pos);
    *this = ans;
    delete_zeroes(data);
    return *this;
}

big_integer operator*(big_integer a, big_integer const &b) {
    return a *= b;
}

big_integer &big_integer::operator<<=(int rhs) {
    int t = rhs / LOG_BASE;
    data.resize(t + data.size());
    for (int32_t i = data.size() - 1; i + t >= 0; i--) {
        if (i >= 0)
            data[i + t] = data[i];
        else
            data[i + t] = 0;
    }
    *this *= (1 << (rhs % LOG_BASE));
    return *this;
}

big_integer operator<<(big_integer a, int b) {
    return a <<= b;
}

big_integer &big_integer::operator>>=(int rhs) {
    bool q = false;
    int t = rhs / LOG_BASE;
    if (*this < 0 && *(this->data.begin()) % 2 == 1)
        *this -= 1;
    for (size_t i = t; i < data.size(); i++) {
        q |= data[i - t];
        data[i - t] = data[i];
        if (t)
            data[i] = 0;
    }
    *this /= (1 << (rhs % LOG_BASE));
    if (q && !pos) {
        *this -= 1;
    }
    return *this;
}

big_integer operator>>(big_integer a, int b) {
    return a >>= b;
}

big_integer big_integer::operator~() const {
    big_integer a = *this;
    return -a - 1;
}

static void my_invert(big_integer &r) {
    for (size_t i = 0; i < r.data.size(); i++) {
        r.data[i] = ~r.data[i];
    }
}

int64_t orr(int64_t a, int64_t b) {
    return (a | b);
}

int64_t andd(int64_t a, int64_t b) {
    return (a & b);
}

int64_t xorr(int64_t a, int64_t b) {
    return (a ^ b);
}

static void binary_operation(big_integer &a, big_integer b, int64_t (*f)(int64_t, int64_t)) {
    big_integer c = a;
    bool q = f(!a.pos, !b.pos);
    if (!c.pos) {
        my_invert(c);
        c.pos = 1;
        c++;
    }
    if (!b.pos) {
        my_invert(b);
        b.pos = 1;
        b++;
    }
    a.data.resize(std::max(size(c), size(b)));
    for (size_t i = 0; i < std::max(size(c), size(b)); i++) {
        a.data[i] = f(get_bool_vect(c.data, i, !a.pos), get_bool_vect(b.data, i, !b.pos));
    }
    if (q) {
        my_invert(a);
        a--;
    }
}


big_integer &big_integer::operator&=(big_integer const &rhs) {
    bool q = (*this < 0 && rhs > 0);
    binary_operation(*this, rhs, andd);
    if (q) {
        *this = -*this;
    }
    return *this;
}

big_integer &big_integer::operator|=(big_integer const &rhs) {
    bool q = (*this >= 0 && rhs < 0);
    binary_operation(*this, rhs, orr);
    if (q) {
        *this = -*this - 2;
    }
    return *this;
}

big_integer &big_integer::operator^=(big_integer const &rhs) {
    bool q1 = (*this >= 0 && rhs < 0), q2 = (*this < 0 && rhs < 0);
    binary_operation(*this, rhs, xorr);
    if (q1) {
        *this = -*this - 2;
    }
    if (q2) {
        *this = -*this;
    }
    return *this;
}

big_integer operator&(big_integer a, big_integer const &b) {
    return (a &= b);
}

big_integer operator|(big_integer a, big_integer const &b) {
    return (a |= b);
}

big_integer operator^(big_integer a, big_integer const &b) {
    return (a ^= b);
}

static int64_t divide(big_integer &a, big_integer const &b) {
    int64_t l = 0, r = BASE, mid;
    while (l < r - 1) {
        mid = (l + r) / 2;
        if (mid * b <= a)
            l = mid;
        else
            r = mid;
    }
    a -= l * b;
    return l;
}

static inline void mov_vect(big_integer &a) {
    for (int32_t i = size(a) - 1; i + 1 >= 0; i--) {
        check_vector(a.data, i + 1);
        if (i >= 0)
            a.data[i + 1] = a.data[i];
        else
            a.data[i + 1] = 0;
    }
}

static void div_int(big_integer &a, int rhs) {
    int64_t r = 0, t;
    for (int32_t i = size(a) - 1; i >= 0; i--) {
        t = a.data[i] + r * BASE;
        a.data[i] = (int64_t) t / rhs;
        r = (int64_t) t % rhs;
    }
    delete_zeroes(a.data);
}


big_integer &big_integer::operator/=(big_integer const &rhs) {
    if (size(rhs) == 1) {
        if (rhs < 0) {
            pos = !pos;
        }
        div_int(*this, rhs.data.back());
        return *this;
    }
    bool q = (this->pos == rhs.pos);
    big_integer rh = rhs;
    rh.pos = 1;
    big_integer res = 0;
    res.data.resize(data.size());
    big_integer r = 0;
    for (int32_t i = size(*this) - 1; i >= 0; i--) {
        if (r >= rh) {
            add(res, divide(r, rh));
        }
        mov_vect(res);
        mov_vect(r);
        add(r, (int)(this->data[i]));
    }
    if (r >= rh)
        add(res, divide(r, rh));
    delete_zeroes(res.data);
    *this = res;
    this->pos = q;
    return *this;
}

big_integer operator/(big_integer a, big_integer const &b) {
    return (a /= b);
}

big_integer &big_integer::operator%=(big_integer const &rhs) {
    *this = *this - *this / rhs * rhs;
    return *this;
}

big_integer operator%(big_integer a, big_integer const &b) {
    return (a %= b);
}