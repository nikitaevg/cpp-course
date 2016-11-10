template <typename T>
void f(T&);

template <typename T>
void g(T const&);

template <typename T>
void h(T&&)

int a;
int const b;
int c();
int const d();

f(a); // T -> int
f(h); // T -> int const
f(c()); // error Так как хочется создать lvalue ссылку на временный объект
f(d()); // error

g(a); //T -> int
g(b); //T -> int
g(c()); //T -> int
g(d()); //T -> int

h(a); // T-> int&   void h(int &); reference collapsing rule
h(b); // T->int const& void h(int const&);
h(c()); // T->int void h(int &&);
h(d()); // void h(int const&&);

template <typename T>
T&& forward(T&& a)
{
	return state_const<T&&>(a);
}

template <typename U>
void f(U&& u)
{
	g(forward<U>(u));
}

int a;
f(a); U->int&
void f(int& u)
{
	g(forward<int&>(u));
	T->int&
	int& forward(int&);
}

f(10);
U->int
void f(int&& u)
{
	forward<int>(u);
}
T->int
int&& forward(int&&);

forward возвращает rvalue или lvalue в зависимости от того, что ему передали. Это perfect-forwarding

Чтобы передать не один тип, а много:
template <typename...U>
void f(U&&...u)
{
	g(froward<U>(u)...);
}

Штука с троеточием -- variadic templates

perfect forwarding -- сохранение r/lvalue, const не const.


variadic функции (до c++11)
int printf(char const* msg, ...);
Можно передать сколько угодно аргументов.

template <typename ... U>
void g(U ... args)
{
	h(args...);

	h0(h1(args...)); эквивалентно
	h0(h1(a0, a1, ...));

	hp(h1(args)...); эквивалентно
	hp(h1(a0), h1(a1), ...);
}
sizeof...(U) -- сколько аргументов у U.

template <typename B>
struct derived : B...
{};

template <typename ... T>
struct all_are_same;

template <typename T0, typename T0, typename ... Ts>
struct all_are_same <T0, T0, Ts...>
{
	static const value = all_are_same<T0, Ts...>::value;
}

template <typename T0>
struct all_are_same<T0>
{
	static const bool value = true;
}


























