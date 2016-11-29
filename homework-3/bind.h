#ifndef bind_h
#define bind_h

#include <vector>
#include <functional>

template <int N>
struct place_holder
{
};
place_holder<0> __first;
place_holder<1> __second;
place_holder<2> __third;
place_holder<3> __fourth;
place_holder<4> __fifth;

#define _1 __first
#define _2 __second
#define _3 __third
#define _4 __fourth
#define _5 __fifth


template <typename F, typename... Args>
struct bind_t
{

private:
    
    typedef std::tuple<Args...> tuple_;
    typedef typename F::result_type R;
    
    F func;
    tuple_ args;
    
    bind_t(F&& func, Args&&... args) :
    func(func),
    args(std::forward<Args>(args)...)
    {
    }
    
    template <typename old_Arg, typename... new_Args>
    old_Arg arg_get(old_Arg&& old_arg, new_Args&&... new_args) const
    {
        return old_arg;
    }
    
    template <typename Fn, typename... old_Args, typename... new_Args>
    typename Fn::result_type arg_get(const bind_t<Fn, old_Args...>& b, new_Args&&... new_args) const
    {
        return b(new_args...);
    }
    
    template <int N, typename... new_Args>
    typename std::tuple_element<N, std::tuple<new_Args...> >::type arg_get(const place_holder<N>, new_Args&&... new_args) const
    {
        return std::get<N>(std::make_tuple<new_Args...>(std::forward<new_Args>(new_args)...));
    }
    
    template <int... N>
    struct sequence
    {};
    
    template <int S, int... N>
    struct unpacker
    {
        typedef typename unpacker<S - 1, S - 1, N...>::type type;
    };
    
    template <int... N>
    struct unpacker<0, N...>
    {
        typedef sequence<N...> type;
    };
    
    template <typename... new_Args, int... S>
    R call_impl1(const sequence<S...>&&, new_Args&&... new_args) const
    {
        return func(arg_get(std::get<S>(args), std::forward<new_Args>(new_args)...)...);
    }
    
    template <typename... new_Args>
    R call(new_Args&&... new_args) const
    {
        return call_impl1(typename unpacker<std::tuple_size<tuple_>::value>::type(), std::forward<new_Args>(new_args)...);
    }
    
    template <typename Fn, typename... Args_>
    friend bind_t<std::function<Fn>, Args_...> bind(const Fn* func, Args_&&... args);

public:
    template <typename... new_Args>
    R operator()(new_Args&&... new_args) const
    {
        return call(std::forward<new_Args>(new_args)...);
    }
    
};

template <typename F, typename... Args>
bind_t<std::function<F>, Args...> bind(const F* func, Args&&... args)
{
    return bind_t<std::function<F>, Args...>(std::function<F>(func), std::forward<Args>(args)...);
}


#endif
