Templates продолжение

partial специализация -- указываем только некоторые, но не все специализации (Int, Int, T)

Пример partial:
template <typename T> 
struct vector
{
	T* data;
	size_t size;
	size_t capacity;
}

Ликбез по темплейтам:
1. primary template:
template <...>
struct name
{

};

2. Специализация (partial) (остаются неразрешенные типы)
template <...> struct name <...> 
{
	
};

3. Полная специализация (explicit специализация)
template <>
struct name <...>
{
	
};

Рассмотрим vector<mytype *> и vector <some_other_type* >
Два типа разные, хотя по сути они одинаковые. Для них создадутся абсолютно одинаковый код для этих разных векторов.
Для переиспользования кода можно сделать так:

template <typename U>
struct vector<U*>
{
	vector_ptr impl;
};

Пример когда в partial специализации больше типов

template <typename A, typename B, typename C>
struct x<A (*)(B), B (*)(C)> {

}

template <typename A, typename B, typename C>
struct x<pair<A, B>, pair<B, C> >
{
	
};


template <typename U, typename V>
void f(U, V);

template <typename A, typename B, typename C>
void f(A(*)(B), B(*)(C));

дефолтные аргументы:

template <typename U (= int), typename V (= int*)>
struct x;

О массивах:

template <typename T, size_t N> (можно передавать и enum и nullptr)
struct array
{
	T data[N];
};

template <typename U>
struct <U, 0>
{};

array это как массив который может копироваться, проверяет всякую хрень. А еще можно создавать массив из нуля элементов

template <unsigned N>
struct factorial 
{
	static const unsigned value = N * factorial <N - 1>::value;
};

template <>
struct factorial<0>
{
	static const unsigned value = 1;
}
 это высчитывается во время компиляции и поэтому нельзя передавать переменные runtime. 
 Все темплейты кешируются и поэтому факториал будет за линию. То есть fact<4> не будет высчитываться дважды. Фиббоначи тоже будет высчитываться за линию.
 compiled time константа это только const int x = 5, без конст или вместо 5 функция то это уже не компайлд тайм.

 Вообще это Тьюринг полный язык и на нем можно написать что угодно

 Неполные или полные типы для темплейтов:

 scoped_ptr - штука которая вызывает деструктор при выходе из области

 template <typename T>
 struct scoped_ptr 
 {
 	~scoped_ptr()
 	{
 		sizeof(*ptr); // проверка complete тип или нет
 		delete ptr;
 	}
 private:
 	T* ptr;
 }

// mytype.h
 struct type;
 struct mytype
 {
 	~mytype();  //если тут написать фигурные скобки, то это не сработает, так как тут type еще инкомплит. А написав фигурные скобки мы захотим инстанциировать деструктор
 	// А деструктор вызывает деструкторы всех членов и инстанциирует ~scoped_ptr. А так мы этого избегаем, инстанциируем в cpp, где type уже комплит.
 	scoped_ptr<type> p;
 }
 ошибки не будет так как темплейт не будет инстанцировать все функции при создании класса а будет их инстанцировать уже при вызове.

//mytype.cpp
 struct type
 {};

 mytype::~mytype()
 {};
Мембер функции инстанцируются только когда мы к ним обращаемся. Инстанироваться -- пройти все проверки.

template <typename T>
struct x;

x <int> a; // -- херня, так как x еще incomplete

template инкомплит пока он не проинстанциирован

template <typename D>
struct base :D::type 
{};

struct xxx
{};

struct derived : base <derived> // derived здесь еще incomplete а значит в :D::type еще нет такого поля так как оно не создано.
{
	typedef xxx type;
}

template <typename D>
struct base :D::type 
{};

struct xxx
{};

template <typename T>
struct derived : base <derived <T> > 
{
	typedef xxx type;
}

derived <int> a; // <-- теперь здесь ошибка, а при объявлении derived все будет ок так как темплейт не будем подставлять пока не будем создавать a;


























