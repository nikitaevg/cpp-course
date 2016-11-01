//The continue of tag dispatching is SFINAE
//Substitution failure is not an error

template <typename C>
void f(C& c, typename C::iterator pos);

template <typename T, size_t N>
void f(T (&c)[N], T* pos); // T (&c)[N] -- Link to array c of type T of size N

//void f(int a[]) -> void f(int* a)

int a[100];
f(a, a + 66); 
// Can't work, because T-> int, n-> 100, C->100 (This was deduction)
// But int[] has  no C::iterator so it is substitution failure. This error is not an error:) And compiler keeps trying to find anther template
// The specialization is chosen the less wider.

template <typename U, typename V>
struct is_same
{
	static bool const value = false;
};

template <typename T>
struct is_same<T, T>
{
	static bool const value = true;
}

template<bool Condition, typename T>
struct enable_if;

template<typename T>
struct enable_if<true, T> {
 typedef T type;
}

template<typename C, typename F>
typename enable_if<is_same<typename container_traits<C>::tag, node_based_tag>::value, void>::type
 erase_if(C& c, F pred) {
 
  }

//Если кондишон не подойдет, он завалится, потому что enable_if будет incomplete,
//	но при этом по SFINAE не завалится, а пойдет искать дальше.


template <typename T>
void f(typename container_traits<T>::tag);

f(vector_like_tag()); // Ошибка так как не может задедьюсить tag в структуру, что логично

// No deduce:

template <typename T>
struct no_deduce
{
	typedef T type;
};
template <typename T>
void f(T, T);

f(5, 6.); // ошибка дедакшана

template <typename T>
void f(vector<T> const& v, T const& value);

vector<unsigned> v;
f(v, 5); // Ошибка
f(v, 5u); // ОК
// но первый случай вообще было бы неплохо пропускать, чтобы Т дедьюсился от первого аргумента
// чтобы подавить дедьюсинг от второго аргумент:

template <typename T>
void f(vector<T> const& v, typename no_deduce<T>::type const& value);
// теперь все будет кастоваться к unsigned. А в начале не работало так как пытался дедьюсить по обоим аргументам и не получалось

// Optimizations
// O0 -- nothing is optimized
// O1 -- easy optimiztions. Mostly not used
// O2 -- common optimization. 































