
#ifndef my_vector_h
#define my_vector_h

#include <stdio.h>
#include <memory>
#include <vector>

template <typename T>
void edit_check(std::shared_ptr<T> &p)
{
    if (!p.unique())
    {
        p = std::make_shared<T>(*p);
    }
}

template<typename T>
struct my_vector
{
private:
    int8_t small;
    std::shared_ptr<std::vector<T> > data;
    T local;
    const static size_t COW_SIZE = 1;
public:
    my_vector()
    {
        small = 0;
    }
    my_vector(my_vector const &other) :
    small(other.small)
    {
        if (small > 1)
            data = other.data;
        else
            local = other.local;
    }
    
    ~my_vector()
    {
        
    }
    
    T& operator[] (size_t i)
    {
        if (small == 1 && i < COW_SIZE)
            return local;
        if (small > 1)
            edit_check(data);
        return (*data)[i];
    }
    
    T const& operator[] (size_t i) const
    {
        if (small <= 1 && i <= 1)
            return local;
        return (*data)[i];
    }
    
    void pop_back()
    {
        if (small <= 1)
        {
            small = 0;
        }
        else
        {
            edit_check(data);
            data->pop_back();
        }
    }
    
    void push_back(T x)
    {
        if (small > 1)
        {
            edit_check(data);
            data->push_back(x);
        }
        else if (!small)
        {
            small = 1;
            local = x;
        }
        else
        {
            small = 2;
            data = std::make_shared<std::vector<T> >();
            data->push_back(local);
            data->push_back(x);
        }
    }
    
    T const& back() const
    {
        if (small <= 1)
            return local;
        return data->back();
    }
    
    my_vector<T>& operator=(my_vector const& rhs)
    {
        small = rhs.small;
        if (small > 1)
            data = rhs.data;
        else
            local = rhs.local;
        return *this;
    }
    
    size_t size() const
    {
        if (small < 2)
            return small;
        else return data->size();
    }
    
    void resize(size_t sz)
    {
        if (small < 2 && !sz)
        {
            small = 0;
        }
        else if (small < 2 && sz >= 2)
        {
            if (!small)
                local = 0;
            data = std::make_shared<std::vector<T> >();
            data->push_back(local);
            small = 2;
            data->resize(sz);
        }
        
    }
    
};

#endif
