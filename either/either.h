#include <algorithm>
#include <functional>
#include <memory>
#include <iostream>

struct emplace_left_t
{};

static emplace_left_t emplace_left;

struct emplace_right_t
{};

static emplace_right_t emplace_right;

template <typename Left, typename Right>
struct either
{
	either(Left rhs) :
		now_left(true),
		in_heap(false) 
	{
		new (&data) Left(std::move(rhs));
	}
	either(Right rhs) :
		now_left(false),
		in_heap(false)
	{
		new (&data) Right(std::move(rhs));
	}
	either(either const& rhs) 
	{
		now_left = rhs.now_left;
		in_heap = false;
		if (now_left) 
		{
			new (&data) Left(rhs.left());
		}
		else 
		{
			new (&data) Right(rhs.right());
		}

	}
	~either() {
		destruct();
	}
	either(either&& rhs)
	{
		now_left = rhs.now_left;
		in_heap = false;
		if (rhs.is_left()) 
		{
			new (&data) Left(std::move(rhs.left()));
		}
		else 
		{
			new (&data) Right(std::move(rhs.right()));
		}
	}

	template<typename ... Args>
    either(emplace_left_t, Args&& ... args)
        : now_left(true),
       	in_heap(false)
    {
        new (&data) Left(std::forward<Args>(args) ...);
    }

    template<typename ... Args>
    either(emplace_right_t, Args&& ... args)
        : now_left(false),
        in_heap(false)
    {
        new (&data) Right(std::forward<Args>(args) ...);
    }

	either() = delete;
	either& operator=(either rhs) // strong
	{
		if (rhs.is_left())
			emplace(emplace_left_t(), std::move(rhs.left()));
		else 
			emplace(emplace_right_t(), std::move(rhs.right()));
		return *this;
	}

	bool is_left()
	{
		return now_left;
	}
	Left& left()
	{
		if (!in_heap) 
			return *reinterpret_cast<Left*>(&data);
		return *reinterpret_cast<std::unique_ptr<Left>*>(&data)->get();
	}
	Left const& left() const
	{
		if (!in_heap) 
			return *reinterpret_cast<const Left*>(&data);
		return *reinterpret_cast<const std::unique_ptr<const Left>*>(&data)->get();
	}

	bool is_right()
	{
		return !is_left();
	}
	Right& right()
	{
		if (!in_heap) 
			return *reinterpret_cast<Right*>(&data);
		return *reinterpret_cast<std::unique_ptr<Right>*>(&data)->get();
	}
	Right const& right() const
	{
		if (!in_heap) 
			return *reinterpret_cast<const Right*>(&data);
		return *reinterpret_cast<const std::unique_ptr<const Right>*>(&data)->get();
	}

	template <typename... T>
	void emplace(emplace_left_t, T&&... args) // strong
	{
		if (is_left())
			emplace_impl<Left>(left(), std::forward<T>(args)...);
		else
			emplace_impl<Left>(right(), std::forward<T>(args)...);
		now_left = true;
	}

	template <typename... T>
	void emplace(emplace_right_t, T&&... args) // strong
	{
		if (is_left())
			emplace_impl<Right>(left(), std::forward<T>(args)...);
		else
			emplace_impl<Right>(right(), std::forward<T>(args)...);
		now_left = false;	
	} 

private:
	bool in_heap;
	bool now_left;
	static const int max_size = std::max({sizeof(std::unique_ptr<Left>), sizeof(Left), sizeof(Right)});
	static const int max_align = std::max({alignof(std::unique_ptr<Left>), alignof(Left), alignof(Right)});
	std::aligned_storage_t<max_size, max_align> data;
	template <typename New, typename Old, typename... T>
	void emplace_impl(Old& oldVal, T&&... args) 
	{
		auto temp = std::make_unique<Old>(oldVal);
		try
		{
			destruct();
			new (&data) New(std::forward<T>(args)...);
			in_heap = false;
		}
		catch(...)
		{
			in_heap = true;
			new (&data) std::unique_ptr<Old>(temp.release());
			throw;
		}
	}

	template <typename T>
	void put_to_heap (T& arg) {
		new (&data) std::unique_ptr<T>(arg);
	}

	void destruct()
	{
		if (now_left) {
			if (!in_heap)
				left().~Left();
			else
				reinterpret_cast<std::unique_ptr<Left>*>(&data)->~unique_ptr();
		} 
		else {
			if (!in_heap)
				right().~Right();
			else
				reinterpret_cast<std::unique_ptr<Right>*>(&data)->~unique_ptr();	
		}

	}
	template <typename L1, typename R1, typename L2, typename R2, typename swL, typename swR>
	friend void swap_impl(std::unique_ptr<swL> a, std::unique_ptr<swR> b, either<L1, R1>& lhs, either<L2, R2>& rhs, bool reverse);
};

template <typename F, typename Left, typename Right>
decltype(auto) apply(F&& f, either<Left, Right> const& arg)
{
	return (arg.is_left() ? f(arg.left()) : f(arg.right()));
}

template <typename F, typename Left, typename Right>
decltype(auto) apply(F&& f, either<Left, Right> & arg)
{
	return (arg.is_left() ? f(arg.left()) : f(arg.right()));
}

template <typename L, typename R, typename swL, typename swR>
void swap_impl(std::unique_ptr<swL> a, std::unique_ptr<swR> b, either<L, R>& lhs, either<L, R>& rhs, bool reverse)
{
	try 
	{
		lhs.destruct();
		rhs.destruct();
		new (&lhs.data) swR(*b);
		new (&rhs.data) swL(*a);
		if (reverse) {
			lhs.now_left = !lhs.is_left();
			rhs.now_left = !rhs.is_left();
		} 
	}
	catch(...) 
	{
		lhs.in_heap = rhs.in_heap = true;
		new (&lhs.data) std::unique_ptr<swL>(a.release());
		new (&rhs.data) std::unique_ptr<swR>(b.release());
		throw;
	}
}

template<typename L, typename R>
void swap (either<L, R> & a, either<L, R>& b) // strong
{
	bool a_l = a.is_left(), b_l = b.is_left();
	if (a_l && b_l) {
		swap_impl(std::make_unique<L1>(a.left()), std::make_unique<L2>(b.left()), a, b, false);
	}
	else if (a_l && !b_l) {
		swap_impl(std::make_unique<L1>(a.left()), std::make_unique<R2>(b.right()), a, b, true);
	}
	else if (!a_l && b_l) {
		swap_impl(std::make_unique<R1>(a.right()), std::make_unique<L2>(b.left()), a, b, true);
	}
	else if (!a_l && !b_l) {
		swap_impl(std::make_unique<R1>(a.right()), std::make_unique<R2>(b.right()), a, b, false);
	}
}