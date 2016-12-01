Возвращаемое значение функционального объекта (задача распространенная для библиотек)

template <typename F>
void g(F const& f)
{
	? val = f();
	auto val = f();
	// Но если надо ретернить, то сложнее.
	decltype(f()) val = f();
}

typeof в c++ до 11, так как некорректно работает с rvalue ( или с чем-то другим)
decltype(f()) val = f(); 

как ретернить
можно 
decltype(F()()) g(F const& f) // если нет пустого конструктора, не сработает
{...}

decltype(f()) g(F const& f) // не скомпилится
{...}

decltype(std::declval<F>()()) g(F const& f);
std::declval<F>() -- возвращает объект типа F. Как -- непонятно. 

x::value_type x::f(value_type) -- у аргумента не надо писать x:: так как на этом этапе компилятор уже знает, что функция из x. А возвращемое может быть из x, но функция не из x.
{...}							

trailing return type
template <typename F>
auto f(F const& f) -> decltype(f()) {...}

В c++03 в функциональных объектах тайпдефили ReturnType. Для функций использовали traits.

В c++14 можно просто написать auto. и не указывать ->

auto f()
{
	return 1 + 2; 
}

сложный случай дедакшана
struct mytype
{
	typedef int value_type;
	value_type get() const;
}
template <typename T>
auto f(T const& obj)
{
	return obj.get();
}
template <typename T>
auto f(T const& obj) -> decltype(obj.get())
{
	return obj.get();
}
Эти два кода не эквивалентны.
В первом он до подставления темплейтов не знает возвращаемое значение. Первая не может отрезаться по сфинае, вторая может, так как там может не быть get.


decltype:
есть две языковые конструкции
decltype(var) -- type of var
decltype(expr) -- expr - rvalue => type of expr
				  expr - lvalue => type&

int a;
decltype(a) b; // int b
decltype((a)) c // int& c

//------------------- продолжение предыдущей лекции

Продолжение сигналов -- value или variable
Она хранит не только функции, но и значения, которые передаются

Пусть есть a, b, c = a + b;
Идея подписать c на изменения a и b

Пусть 
int[] a;
size_t b;
int c = a[b]; 
c подписан на a и b, b подписан на a
-Просто на сигналах нельзя реализовывать. Так как до c дойдет два раза сигнал.
Во избежание этого надо топсортить все вершины и апдейтить в порядке топсорта.

-Есть еще проблемы. Пусть 
int[] a;
int[] b;
b[i] = f(a[i]);
Для этого можно юзать incremental update(?)

-Херня с с многопоточностью. 
Решение -- lazy update / eager update (?)

-reactive update
-topological sort during update
-increment capture
-threading
-lazy update / eager update
-cyclic dependecy graph

(не понятны некоторые аспекты, надо разобраться)
	