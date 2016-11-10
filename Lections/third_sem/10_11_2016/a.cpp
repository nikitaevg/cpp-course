Продолжение про копирование

Пусть дорого копируется:
struct mytype
{
	int* a;
	int *b;
};

mytype f()
{
	mytype result;
	...
	return result;
}

Что если переписать на

void f(mytype& result)
{
	...
}
mytype a;
f(a);

При RVO код будет примерно одинаковым, притом первую функцию можно оптимизировать так:
генерируемый код примерно:
void f(void* result); тут компилятор может считать, что result не алиасится.

move-constructor-ы

struct mytype
{
	mytype();
	mytype(mytype const&); & -- lvalue reference //(1)
	mytype(mytype &&); && -- rvalue reference	//(2)
};

void bar(mytype const&);
void bar(mytype &&);

mytype foo();

int main()
{
	mytype a;
	mytype b(a); 	 //(1)
	mytype c(foo()); //(2)	(c++17 что-то не так)

	bar(a); // Первая перегрузка bar
	bar(foo()); // Вторая перегрузка bar

	mytype&& c = a; //error
	// Вообще rvalue ссылки ничем почти не отличаются от lvalue ссылок
}

void foo()
{
	mytype a = barl
	...
	v.push_back(a); // вызывается от lvalue, то есть не move. Как сделать в move?
	v.push_back(static_cast<mytype&&>(a)); // Кастуем когда знаем, что a не будет больше использоваться. Плохой вариант, лучше тот, что снизу
	v.push_back(std::move(a)); // Лучше, так как меньше писать и что-то еще
}

Переменные по стандарту lvalue. То есть:
void push_back(T&& a)
{
	++size;
	void* placement = ...;
	new (placement) T(std::move(a)); // Положить T в placement (кек)
}

void push_back(T const& a)
{
	++size;
	void* placement = ...;
	new (placement) T(a);
}

//-----------------------------

void push_back(T a) // Если передать rvalue, то не будет копирования, и этот код будет эквивалентен предыдущим двум строчкам. Этого кода в два раза меньше
// Но у него move всегда вызывается. Обычно move работает быстро, но если к примеру массив надо мувать, то будет плохо.
{
	+=size;
	void* placement = ...;
	new(placement) T(std::move(a));
}

Как надо писать operator=. Эта реализация продолжит хорошо работать с rvalue ссылками.
mytype& operator=(mytype rhs)
{
	//clear;
	swap(rhs);
	return *this;
}

mytype a = ...
mytype b = ...

a = std::move(b); //Если писать оператор присваивания без clear, то фактически там будет написано swap(a, b), так как передастся ссылка, а не копия в operator=.
// А пользователь может захотеть, что в a память освобождается, а не кладется в b.

std::move не обязательно мувается. Если нет move-конструктора, то будет только копирование.

rvalue помог сделать нормальный auto_ptr

unique_ptr(unique_ptr&&);


Рассмотрим снова file_descriptor
struct file_descriptor
{
	int fs;
};

vector <shared_ptr<file_descriptor> >
vector<file_descriptor*>
ptr_vector<file_descriptor>
vector<int>
Это было до c++11

Теперь 
vector<unique_ptr<file_descriptor> > // Это умный указатель, RAII, 
vector<file_descriptor> // Теперь мы будем не копировать, а мувать file_descriptor, а это можно

Пример использования unique_ptr
unique_ptr<mytype> a;
mytype *b = c.get();
Но владеть все равно будет a;

//----------------------------------------------
typedef int& x;
typedef x& y;
// y == int& reference collapsing rule
&+&->&
&+&&->&
&&+&->&
&&+&&->&&


С c++11 или 14
make_unique<mytype>(1, 2, 3) 
реализовано как 
std::unique_ptr<mytype>(new mytype(1,2,3)); Теперь даже new плохо писать.

Реализация unique_ptr

template<typename T>
unique_ptr<T> make_unique()
{
	return unique_ptr<T>(new T());
}




























	