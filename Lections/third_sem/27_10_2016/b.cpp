Варианты хранения некопируемых объектов

vector<file_descriptor*> a; медленно

vector<shared_ptr<file_descriptor> > a; более менее

ptr_vector<file_descriptor> a; почему-то не очень

auto_ptr -- возвращать из функции указатель.

struct auto_ptr
{
	auto_ptr(auto_ptr& other) //зануляет правую часть. Для удовлетворения RAII. К примеру если написать f() то будет ликать память если использовать обычный указатель
	:ptr(other.ptr)
	{
		other.ptr = nullptr;
	}
}

Пример:
auto_ptr<int> f()
{
	auto_ptr<int> result(new int(42));
	return result;
}

Вектор auto_ptr -- херня, так как при копировании то, на что указывал auto_ptr обнулится

В c++11 добавили move-конструктор и move-присваивание, которые правую часть убирают. Он предпочтительнее, так как эта операция дешевле.

push_back(g()); можно использовать move вместо копирования

void push_back(value_type const&);
void push_back_move(value_type&);

Return value optimization
Как понять когда можно использовать move?
Если rvalue, то можно, иначе нельзя

void f(T);

f(g()); // no copy
T a;
f(a); //copy

T g(); <=> void g(void* storage) в си

Примеры некопирования объекта

T a = g();

char a_buf[];
g(a_buf);

//-----------------------

f(g());

char buf[sizeof(T)];
g(buf);
f(buf);


//-----------------------

T a;
a = g();


char a_buf[...];
a_ctor(a_buf);
char tmp[...];
g(tmp);
operator(a_buf, tmp);


//-----------------------

return value optimization

T g()
{
	return T(1, 2, 3);
}

void g(void* result)
{
	T_ctor(result, 1, 2, 3); вместо создания объекта от 1,2,3 и потом копирования и потом вызова деструктора
}

Named return value optimization

T g()
{
	T a;
	...
	return a;
}

преобразуется в 

void g(void* result)
{
	try
	{
		T_ctor(result);
		...
		...
		...
		вместо return ничего. То есть кладем только в result
	}
	catch(...)
	{
		T_dtor(result);
		throw;
	}
	// try catch блок нужен для того, чтобы если случится эксепшн не создавался в result что-либо. Иначе это было бы не эквивалентно
}































