Анонимные функции.

Пример:
std::vector<int> v;

std::find_if(v.begin(), v.end(), какой-то предикат)
Что может служить предикатом:

1. 
bool is_even(int a)
{
	return (a % 2) == 0;
}
нет оптимизации, так как функция будет известна только в рантайме (динамический полиморфизм)
вставляем is_even

2. 
struct is_even_t
{
	bool operator() (int a) const
	{
		return (a % 2) == 0;
	}
}
вставляем is_even()
Тут компайл-тайм подстановка. Так как find_if темплейтный, для класса он будет инстанцироваться от класса is_even_t -- инстанциация будет во время компиляции.
А функция не подставляется во время инстанциации -- там просто указатель на функцию, какую-нибудь функцию

Если надо использовать переменную из скоупа, можно передать ее в конструктор предиката. Пусть переменная  n

struct is_multiple_of {
	is_multiple_of(int n) : n(n) {}

 	bool operator()(int a) {
  		return a % n == 0;
	}

	private: 
		int n;
};

c++11
std::find_if(v.begin(), v.end(), [n](int a){return (a % n) == 0});

Лябмда-функции генерят практически тот же код, что и там сверху с классом.

[] -- capture list -- набор данных для захвата
() -- аргументы функции
{} -- тело

Анонимные потому что на них не можем ссылаться дальше!!

Примеры капчуров:
[] - пустой
[a,b,c,&d,&e,&f](){} -- первые три переменные копируются по значению, вторые три копируются по ссылке

[]()->int{return 3;} -- явное указание возвращаемого аргумента

шорткаты:
[=, &d, &e, &f](){} -- копирует все аргументы, к которым обращается функция по значению, кроме def
[&, a, b, c](){} -- то же, только по ссылке кроме abc.

[]()mutable{} -- какая-то хрень, типа у класса, который сгенерился, у круглых скобок не const возвращаемое значение.

? func;
if (cond)
	func = [..](int a)->void{...};
else
	func = [..](int a)->void{...};

Если не хочется создавать классы для каждой новой лябмды (типа хотим сортировать разные вещи, но производительность не важна, а количество кода хочется уменьшить)

std::function<void(int)> func; (еще в c++03)
Типы и возвращаемые значения не обязательно должны совпадать.

Напишем function только для function<void(int, int)> чтобы не засорять код. В общем случае надо общие случаи рассматривать (кек).

// наивная реализация
struct function 
{
	typedef void (*deleter_t)(void*);
	typedef void (*caller_t)(int, int);
	template <typename F>
	function (F func)
	:ptr(new F(std::move(func))),
	deleter(&function::delete_obj<F>), //Таким образом мы как бы запомнили тип, чтобы его нормально удалить. Создался делитер для F
	caller(*function::call_obj<F>);
	{}

	void operator()(int a, int b) const
	{
		return caller(ptr, a, b);
	}

	void *ptr;

private:
	template <typename F>
	static void delete_obj(void *p)
	{
		delete static_cast<F*>(p);
	}
	template <typename F>
	static void call_obj(void *p, int a, int b)
	{
		return static_cast<F&>(*p)(a, b);
	}
}

Так как F может быть большим, он не может поместиться в function, поэтому придется по-любому выделять память (нехилое размышление. В статическую память может не поместиться что-то большое)
Что важно -- в классе не хранится F, он не темплейтный и один на все функции, которые примерно похожи (аргументы и возвращаемое значения кастуются). То есть его можно юзать вместо вопросика наверху.

Ликбез по typedef
int a;
эквивалентно
typedef int type;
type a;

void (*b)(int, int);
эквивалентно
typedef void (*type)(int, int);
type b;

Писали для function<void(int, int)>
Что такое void(int, int)? Это тип функция (не указатель)

Напишем общий случай
template <typename Sig>
struct function;

template <typename R, typename... Params>
struct function<R, (Params...)>
{
	typedef void (*deleter_t)(void*);
	typedef R (*caller_t)(Params...);;
	template <typename F>
	function (F func)
	:ptr(new F(std::move(func))),
	deleter(&function::delete_obj<F>), //Таким образом мы как бы запомнили тип, чтобы его нормально удалить. Создался делитер для F
	caller(&function::call_obj<F>);
	{}

	R operator()(Params...p) const
	{
		return caller(ptr, std::forward<Param>(p)...);
	}

	void *ptr;

private:
	template <typename F>
	static void delete_obj(void *p)
	{
		delete static_cast<F*>(p);
	}
	template <typename F>
	static void call_obj(void *p, Params...p)
	{
		return static_cast<F&>(*p)(std::forward<Params>(p)...);
	}
}

Та херня с тремя точками (p) нельзя ее использовать без трех точек где-то недалеко. 

Кстати function не тайп-сейф внутри (есть касты) но снаружи он выглядит круто.