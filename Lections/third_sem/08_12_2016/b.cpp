как релизовать оператор присваивания с strong exception safety
Double-buffered variant
variant<A, B, C>

Может хранить как объекты A, B, C либо A*, B*, C*. Вторые типы будут храниться на хипе. Этот вариант маловариантен
Для strong exception, если конструктор бросает, объект, который хранился до этого, оказывается теперь на хипе. Если без этого
делать, то если конструктор бросит, то старый объект будет потерян.

В такой реализации which и get будут сложнее, будут возникать ветки ифов.
Есть другая реализация
explicit singular state 4 состояния. В случае падения возвращается какой-то дефолтный класс. Типа A, B, C, default
rare singular state.Тоже 4 стейта, но для пользователя как бы три. Четвертый стейт можно узнать через функцию is_valid Просто флажок.

Boost.TypeErasure
Хранить классы с какими-нибудь констрейнтами
Пример:

any_<default_constuctible,
	copyable,
	incrementable,
	referable<int> > a, b;
a = v.begin();
b = v.end();
Типа интерфейсы

std::list<int> a;
std::sort(a.begin(), a.end());
Такое не должно работать. Чтобы не заходить далеко по функциям (до первого вызова от итератора+=) можно использовать
static_assert
SFINAE
Пусть надо проверить что a==*b работает

template <typename A, typename B>
auto f(A a, B b) -> std::enable_if<(sizoef(a==*b), true), void>::type
оператор запятая -- выполняет слева направо, возвращая последний результат. В for for(;;i++, j++) именно этот оператор

Непонятно, как делать констрейнты хорошими, удобными, если ты автор библиотеки. 
user<->interface<->library. Последнюю стрелку как констрейнт сложно сделать. Можно переконстрейнтить, можно недоконстрентить.

Еще один вариант -- 
static_if

template <typename T>
static_if(is_unsigned<T>::value)
void f(T a)
{

}
Он не понравился. 
Другой вариант -- concepts library (declaration-based constraints)

template <typename T>
require(has_push_back<T>)
void f(T a)
{
	a.push_back(0);
}

как задавать наличие функции у типа?
template <typename T>
concept has_push_back
{
	typename T::value_type;
	void push_back(T::value_type);
}

Еще один вариант -- concept expression-based constraints

template <typename T>
concept predicate = ??? {U u; V v; u(*v);}

концепты можно разделить на explicit и implicit 
Должны ли классы говорить, что они удовлетворяют какому-то концепту, или нет. Наличие функции является гарантией того, что класс удовлетворяет концепту












