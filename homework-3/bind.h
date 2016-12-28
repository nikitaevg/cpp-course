#ifndef bind_h
#define bind_h

#include <functional>
#include <string>

template <int N>
struct place_holder
{
};
place_holder<0> _1;
place_holder<1> _2;
place_holder<2> _3;
place_holder<3> _4;
place_holder<4> _5;

template <typename Fp, typename... Args>
struct bind_t
{
    
private:
    
    typedef typename std::decay<Fp>::type F;
    typedef std::tuple<std::decay_t<Args>...> tuple_;
    
    F func;
    tuple_ args;
    
    template <typename F_, typename... Args_>
    bind_t(F_&& func, Args_&&... args) :
    func(std::forward<F_>(func)),
    args(std::forward<Args_>(args)...)
    {
    }

    template <typename old_Arg, typename... new_Args>
    decltype(auto) arg_get(old_Arg& old_arg, new_Args&...)
    {
        return old_arg;
    };
    
    template <typename Fn, typename... old_Args, typename... new_Args>
    decltype(auto) arg_get(bind_t<Fn, old_Args...>& b, new_Args&... new_args)
    {
        return b(new_args...);
    }
    
    template <int N, typename... new_Args>
    decltype(auto) arg_get(place_holder<N>&, new_Args&... new_args)
    {
        return std::get<N>(std::forward_as_tuple(new_args...));
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
    auto call_impl1(const sequence<S...>&, new_Args&&... new_args)
    {
        return func(arg_get(std::get<S>(args), new_args...)...);
    }
    
    template <typename... new_Args>
    auto call(new_Args&&... new_args)
    {
        return call_impl1(typename unpacker<std::tuple_size<tuple_>::value>::type(), std::forward<new_Args>(new_args)...);
    }
    
    template <typename F_, typename... Args_>
    friend bind_t<F_, Args_...> bind(F_&& func, Args_&&... args);
    
public:
    template <typename... new_Args>
    auto operator()(new_Args&&... new_args) 
    {
        return call(std::forward<new_Args>(new_args)...);
    }
    
};

template <typename F, typename... Args>
bind_t<F, Args...> bind(F&& func, Args&&... args)
{
    return bind_t<F, Args...>(std::forward<F>(func), std::forward<Args>(args)...);
}


#endif