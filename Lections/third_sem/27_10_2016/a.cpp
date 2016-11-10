C++11:
right-values reference

Появились контейнеры. 

Для push_back нужен оператор копирования, так как в векторе хранятся объекты по значению и их нужно копировать в память по такому-то адресу.

К примеру, file_descriptor нельзя копировать. Можно доопределить копирование как будто по ссылке (все копии данного объекта указывают на один файл)
Вообще достаточно много некопируемых объектов. До 11 хранили векторы указателей.

В 11 появился emplace_back -- создание объекта в самом векторе, без копирования

Как устроен shared_ptr:
shared_ptr<T> a;
shared_ptr<T> b(new T); теперь этот объект это овнер 

shared_ptr<T> c = b;
a = b;
a.reset();
a.reset(new T); //mb huinya
*a
a->f();

shared_ptr<T> b(new T): Создает объект типа T. Shared_ptr состоит из указателя на T и указателя на ref_conter. Оба, причем, общие, то есть при копировании 
копируются указатели и ref_counter увеличивается. В ref_counter есть указатель на объект для того чтобы не скакать по памяти(?). 

T* a = new T;
shared_ptr<T> b(a);
shared_ptr<T> c(a); Это хрень, так как будет указывать на один и тот же объект, а ref_counter будет = 1. При удалении все сломается

T* a = new T;
shared_ptr<T> b(a);
shared_ptr<T> c(b); // С виду может показаться что не безопасный код, но на деле это не так из-за того, что a делетнется если будет эксепшн.

shared_ptr(T* raw) : ptr(raw)
{
	try
	{
		ref_counter_ = new ref_counter();
	}
	catch(...)
	{
		delete raw;
		throw;
	}
}

В ref_counter хранится ptr на объект и кастомный Deleter (по умолчанию будет просто штука, вызывающая delete). Делетер должен иметь оператор () который принимает T*

struct pair
{
	int x, y;
};
shared_ptr<pair> a;

struct x
{
	ref_counter rc;
	T data;
}; Будет создаваться структура x, а на ее поля будет ссылаться из шэрда.

Как работает make_shared:
struct mytype
{
	mytype(int, int, int);
};

shared_ptr<mytype> a = make_shared<mytype>(1, 2, 3);
Это лучше, чем вызывать конструктор от new. Пример:
void f(shared_ptr<mytype>, shared_ptr<mytype>);

f(shared_ptr<mytype>(new mytype), shared_ptr<mytype>(new mytype)); // Этот код не эксепшн сэйв, то если компилятор захочет сначала выделить память, а потом создавать
// уже шэрд, то может быть утечка.

Касты шэрда.
static_pointer_cast<T>(a);
Есть и dynamic и const и т. д.
Сохраняют ref_counter и кастят указатель.


weak_ptr:
ptr, который нельзя разыменовывать
Позволяет ссылаться на тот же объект, на который ссылается shared_ptr, ссылается на ref_counter. 
На самом деле в ref_counter хранится счетчик weak_ptr и shared_ptr (но это тоже неправда, какая-то хрень с многопоточностью)
То есть data хранится пока shared_ptr_counter > 0 а ref_counter пока weak_ptr_counter > 0.
Это может быть исползовано в кэше, потому что если использовать шэрды, то при удалении объекты будут все равно оставаться в памяти.
Чтобы им пользоваться, надо его сделать shared. Этот shared продлит жизнь объекту. Если бы можно было разыменовывать, то такой код мог бы ломаться
weak_ptr a;
a->f();
// может сломаться, так как счетчик стронгов(шэрдов) удалится и объект будет нулом. Из-за многопоточности. Поэтому надо продлевать жизнь объекту, создавая стронг.
a->g();




































