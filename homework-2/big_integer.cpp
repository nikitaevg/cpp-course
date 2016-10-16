#include <iostream>
#include <sstream>
#include "big_integer.h"
#include <vector>

const int START_SIZE = 4;
const long long BASE = ((int64_t) 1 << (int64_t) 31);
const int32_t LOG_BASE = 31;
int cnt = 0;

static inline int64_t get_bool_vect(std::vector<int64_t> const &a, size_t i, bool q) {
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

static inline void edit_check_ptr(std::shared_ptr<big_integer::impl> &p) {
    if (!p.unique()) {
        p = std::make_shared<big_integer::impl>(*p);
    }
}

static inline void add(big_integer &a, big_integer const &b) {
    edit_check_ptr(a.num);
    int64_t carry = 0;
    size_t sb = size(b);
    for (size_t i = 0; i < sb || carry; i++) {
        check_vector(a.num->data, i);
        a.num->data[i] = (a.num->data[i] + (i < size(b) ? b.num->data[i] : 0) + carry);
        carry = 0;
        if (a.num->data[i] >= BASE) {
            a.num->data[i] -= BASE;
            carry = 1;
        }
    }
}

static inline void sub(big_integer &a, big_integer const &b) {
    edit_check_ptr(a.num);
    int64_t carry = 0;
    size_t sb = size(b);
    for (size_t i = 0; i < sb || carry; i++) {
        check_vector(a.num->data, i);
        a.num->data[i] -= (i < size(b) ? b.num->data[i] : 0) + carry;
        carry = 0;
        if (a.num->data[i] < 0) {
            a.num->data[i] += BASE;
            carry = 1;
        }
    }
}

big_integer::big_integer()
{
    num = std::make_shared<impl>();
}

big_integer::big_integer(big_integer const &other) {
    num = other.num;
}

big_integer::big_integer(int x) {
    num = std::make_shared<impl>();
    int64_t a = x;
    this->num->pos = true;
    if (a < 0) {
        this->num->pos = false;
        a = -a;
    }
    while (a) {
        num->data.push_back(a % BASE);
        a /= BASE;
    }
}

big_integer::big_integer(std::string const &str) {
    num = std::make_shared<impl>();
    bool q = true;
    std::string s = str;
    this->num->pos = true;
    if (*s.begin() == '-') {
        q = false;
    }
    big_integer power = 1;
    *this = 0;
    for (int32_t i = str.size() - 1; i >= !q; i--) {
        *this += (str[i] - '0') * power;
        power *= 10;
    }
    this->num->pos = q;
}

big_integer::~big_integer() {
    
}

static int compare_abs(big_integer const &a, big_integer const &b) {
    big_integer x = a;
    big_integer y = b;
    edit_check_ptr(x.num);
    edit_check_ptr(y.num);
    x.num->pos = 1;
    y.num->pos = 1;
    if (x < y)
        return -1;
    else if (x == y)
        return 0;
    else
        return 1;
}

static inline size_t size(big_integer const &a) {
    return a.num->data.size();
}

std::string to_string(big_integer const &a) {
    std::string ret = "";
    big_integer ten = 10;
    big_integer t = a, temp;
    edit_check_ptr(t.num);
    t.num->pos = true;
    bool q = false;
    while (t != 0) {
        temp = (t % 10);
        ret = ret + (char) (temp.num->data[0] + '0');
        t /= 10;
    }
    std::reverse(ret.begin(), ret.end());
    if (!a.num->pos && ret != "")
        ret = "-" + ret;
    if (ret == "")
        ret = "0";
    return ret;
}

std::ostream &operator<<(std::ostream &s, big_integer const &a) {
    return s << to_string(a);
}

big_integer &big_integer::operator=(big_integer const &other) {
    num = other.num;
    return *this;
}

big_integer &big_integer::operator+=(big_integer const &rhs) {
    num->data.resize(std::max(num->data.size(), size(rhs)));
    if (this->num->pos == rhs.num->pos) {
        add(*this, rhs);
    } else if (compare_abs(*this, rhs) >= 0) {
        sub(*this, rhs);
    } else {
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
        if ((i >= size(a) && b.num->data[i]) || (i >= size(b) && a.num->data[i]) ||
            (i < size(a) && i < size(b) && a.num->data[i] != b.num->data[i]))
            return false;
        q |= get_bool_vect(a.num->data, i, 0) | get_bool_vect(b.num->data, i, 0);
    }
    if (q && a.num->pos != b.num->pos)
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
    if (a.num->pos != b.num->pos) {
        return !a.num->pos;
    }
    for (int32_t i = sz - 1; i >= 0; i--) {
        if ((i >= size(b) && a.num->data[i]) || (i < size(a) && i < size(b) && a.num->data[i] > b.num->data[i]))
            return false;
        else if ((i >= size(a) && b.num->data[i]) || (i < size(a) && i < size(b) && a.num->data[i] < b.num->data[i]))
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
    edit_check_ptr(ret.num);
    ret.num->pos = !ret.num->pos;
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
    edit_check_ptr(num);
    big_integer ans;
    ans.num->data.resize(size(*this) + size(rhs));
    int64_t t, carry;
    size_t sz_t = size(*this), sz_b = size(rhs);
    for (size_t i = 0; i < sz_t; i++) {
        carry = 0;
        for (size_t j = 0; j < sz_b || carry; j++) {
            t = ans.num->data[i + j] + num->data[i] * (j < sz_b ? rhs.num->data[j] : 0) + carry;
            ans.num->data[i + j] = (t % BASE);
            carry = t / BASE;
        }
    }
    ans.num->pos = (num->pos == rhs.num->pos);
    *this = ans;
    delete_zeroes(num->data);
    return *this;
}

big_integer operator*(big_integer a, big_integer const &b) {
    return a *= b;
}

big_integer &big_integer::operator<<=(int rhs) {
    edit_check_ptr(num);
    int t = rhs / LOG_BASE;
    num->data.resize(t + num->data.size());
    for (int32_t i = num->data.size() - 1; i + t >= 0; i--) {
        if (i >= 0)
            num->data[i + t] = num->data[i];
        else
            num->data[i + t] = 0;
    }
    *this *= (1 << (rhs % LOG_BASE));
    return *this;
}

big_integer operator<<(big_integer a, int b) {
    return a <<= b;
}

big_integer &big_integer::operator>>=(int rhs) {
    edit_check_ptr(num);
    bool q = false;
    int t = rhs / LOG_BASE;
    if (*this < 0 && *(this->num->data.begin()) % 2 == 1)
        *this -= 1;
    for (size_t i = t; i < num->data.size(); i++) {
        q |= num->data[i - t];
        num->data[i - t] = num->data[i];
        if (t)
            num->data[i] = 0;
    }
    *this /= (1 << (rhs % LOG_BASE));
    if (q && !num->pos) {
        *this -= 1;
    }
    return *this;
}

big_integer operator>>(big_integer a, int b) {
    return a >>= b;
}

big_integer big_integer::operator~() const {
    big_integer a = *this;
    edit_check_ptr(a.num);
    return -a - 1;
}

static void my_invert(big_integer &r) {
    edit_check_ptr(r.num);
    for (size_t i = 0; i < r.num->data.size(); i++) {
        r.num->data[i] = ~r.num->data[i];
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
    edit_check_ptr(a.num);
    edit_check_ptr(b.num);
    big_integer c = a;
    edit_check_ptr(c.num);
    bool q = f(!a.num->pos, !b.num->pos);
    if (!c.num->pos) {
        my_invert(c);
        c.num->pos = 1;
        c++;
    }
    if (!b.num->pos) {
        my_invert(b);
        b.num->pos = 1;
        b++;
    }
    a.num->data.resize(std::max(size(c), size(b)));
    for (size_t i = 0; i < std::max(size(c), size(b)); i++) {
        a.num->data[i] = f(get_bool_vect(c.num->data, i, !a.num->pos), get_bool_vect(b.num->data, i, !b.num->pos));
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
    edit_check_ptr(a.num);
    for (int32_t i = size(a) - 1; i + 1 >= 0; i--) {
        check_vector(a.num->data, i + 1);
        if (i >= 0)
            a.num->data[i + 1] = a.num->data[i];
        else
            a.num->data[i + 1] = 0;
    }
}

static void div_int(big_integer &a, int rhs) {
    edit_check_ptr(a.num);
    int64_t r = 0, t;
    for (int32_t i = size(a) - 1; i >= 0; i--) {
        t = a.num->data[i] + r * BASE;
        a.num->data[i] = (int64_t) t / rhs;
        r = (int64_t) t % rhs;
    }
    delete_zeroes(a.num->data);
}


big_integer &big_integer::operator/=(big_integer const &rhs) {
    edit_check_ptr(num);
    if (size(rhs) == 1) {
        if (rhs < 0) {
            num->pos = !num->pos;
        }
        div_int(*this, rhs.num->data.back());
        return *this;
    }
    bool q = (this->num->pos == rhs.num->pos);
    big_integer rh = rhs;
    edit_check_ptr(rh.num);
    rh.num->pos = 1;
    big_integer res = 0;
    res.num->data.resize(num->data.size());
    big_integer r = 0;
    size_t sz = size(*this) - 1;
    for (int32_t i = sz; i >= 0; i--) {
        if (r >= rh) {
            add(res, divide(r, rh));
        }
        mov_vect(res);
        mov_vect(r);
        add(r, (int) (this->num->data[i]));
    }
    if (r >= rh)
        add(res, divide(r, rh));
    delete_zeroes(res.num->data);
    *this = res;
    this->num->pos = q;
    return *this;
}

big_integer operator/(big_integer a, big_integer const &b) {
    return (a /= b);
}

big_integer &big_integer::operator%=(big_integer const &rhs) {
    edit_check_ptr(num);
    *this = *this - *this / rhs * rhs;
    return *this;
}

big_integer operator%(big_integer a, big_integer const &b) {
    return (a %= b);
}
