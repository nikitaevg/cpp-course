Template

До templatе хранили void* к которому можно скастить что угодно. Это было неэкономно но иногда норм (хранить чары в 8 раз менее эффективно)

Можно еще макросами Лол.
#define DECLARE.VECTOR(type)		\
	struct vector_ ##type			\
	{								\
		void push_back(type const&);\
	};
Проблемы: unsigned long проеб и если 2 typedef на лонг то векторы будут разными.

templatе <typename T> struct vector
{
	void push_back(T const&);
	T const& back() const;
}

vector<int> a;
a.push_back(5);

Функции с темплейтами
template <typename T>
T max (T a, T b)
{
	return a > b ? a : b;
}

max<int>(5, 6);
max(5, 6); T выводится из аргументов
max(5, 6L); error

Функции темплейт (и структуры) (их реализации) пишутся только в хедере потому что линковка не заходит с темплейтами, так как компилятору надо знать на каждом вызове что за функция.
Потому что иначе может возникнуть так что функцию будут вызывать с типами которые находятся в единице трансляции a а само тело функции в b. 
Темплейт функции по умолчанию inline.

templatе <typename T>
void swap (T& a, T& b)
{
	T tmp = a;
	a = b;
	b = tmp;
}

templatе <typename T> struct vector
{

};

template <> struct vector<bool> //конкретная реализация вектора для булов
{

};

































