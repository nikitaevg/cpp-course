Двухфазный поиск имен

a b(c); -- это либо объявление функции либо создания b типа a 

void f()
{
	(a)-b; -- либо a-b либо каст -b к a

	a (b); по стандарту если что-то может быть прочитано как декларация значит это декларация. Поэтому тут это декларация.

	a < b, c > d; это либо темплейт либо сравнение. Для разрешения надо узнать a это темплейт или нет.
}


template <typename T>
void f()
{
	int a(T::xxx);
	(T::xxx) - b;
	T::xxx(b);

	T::yyy < b, c > d;
}

То что зависит от темплейта называется зависимым именем (типом, переменной итд)

Типы зависимых имен:
1. T::name 
2. x<T>::name undetermined specialization (у x может быть name либо типом либо еще чем-нибудь)

typename -- сказать компилятору что перед ним тип. Иначе он будет считать что это не тип а выражение (или имя).

(typename T::xxx) - b; -- каст к xxx

T::template yyy<b,c>::template zzz<int> -- ::template указывает комплиятору что yyy это темплейт. Иначе он будет думать что там сравнения.

template <typename T>
void f(T)
{
	mytype a;
}

struct mytype
{
	
};

int main()
{
	f(5);
}

У MS это зайдет, так как они до инстанцирования темплейта не проверяют функцию, а только запоминают ее как последовательность токенов. То есть тут mytype не будет еще
создан а f зайдет. А gcc смотрит функцию f и не знает что за mytype.

Двухфазный поиск: 
1 фаза -- не вставляя темплейт смотрит на функцию и пытается это разобрать
2 фаза -- подставление темплейтов и просмотр уже функций с темплейтами (инстанциирование)


export template

c++98/c++03
поддерживалась компилятором cameau который типа самый соответсвующий стандарту

Позволяла объявлять темплейты в h, а тело делать в cpp


пример:

//a.cpp

#include <vector>

struct mytype
{
	int a;
};

std::vector<mytype> x;

//b.cpp

#include <vector>

struct mytype
{
	double a;
};

std::vector<mytype> y;

Проблема в том что при линковке он хочет создать один и тот же вектор, хотя они должны быть разными. Поэтому ошибка на этапе линковки.

explicit ассоциации
string это не тип а typedef basic_string<char> string;
typedef basic_string<wchar_t> wstring;

extern template struct basic_string<char>; инстанцирует template только один раз. К примеру string: так как он темплейтный, он весь написан в h, и если 
везде писать include<string>, весь код будет прописываться, а это херня. Поэтому делают extern template struct. Мы используем string почти всегда с 
чарами, он пропишется один раз в файле и после этого вместо подстановки темплейтной реализации будет линковаться просто с файлом в котором прописан basic_string<char>
Пример: 

// a.h
template<typename T>
void ReallyBigFunction()
{

}

//a.cpp
#include "a.h"
void something1()
{
    ReallyBigFunction<int>();
}

//b.cpp
#include "a.h"

extern template void ReallyBigFunction<int>();
void something2()
{
    ReallyBigFunction<int>();
}






































