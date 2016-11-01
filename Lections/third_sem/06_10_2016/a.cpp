tag_dispatching!! (гуглится)
SFINAE

STL

STL состоит из трех штук:
1. Контейнеры (vector, set, list, deque) [undorered][multi]{set, map}
2. Алгоритмы (find, sort, lower_bound, upper_bound, copy ...)
3. Итераторы
	Категории итераторов
	Самые слабые -- Input Iterator и Output Iterator -- можем пройти вперед только один раз и все (типа данные из инернета)
	ForwardIterator -- поддерживают такие операции: == != copy ++ *. Пример -- односвязный
	BiDiretional Iterator -- reverse, --
	RandomAccessIterator += -= +N -N


template <typename It, typename P>
	It find (It first, It last, P pred)
{
	while (first != last && !pred (*first))
		++first;
	return first;
}

template <typename T>
struct struct iterator_traits
{
	typedef typename T::value_type value_type;
}

template <typename U>
struct iterator traits <U*>
{
	typedef U value_type;
}

template <typename InputIterator,
		  typename OutputIterator>
OutputIterator copy (InputIterator first,
	Input Iterator last,
	OutputIterator out) // out должен быть вне first и last по соглашению
{
	while (first != last)
		*out += *first==;
	return out;
}

template <typename T> // Это не может быть любым типом, так как если тип не может копироваться
// побайтово, то это не зайдет. Надо как-то специлизировать T
T* copy (T const* first,
	T const& last,
	T* out)
{
	memcpy (out, first, (last - first) * sizeof(T));
	return out + (last - first);
}

template< typename ForwardIterator, typename Cat>
void advance_imple (ForwardIterator& it, size_t n,
					Cat)
{
	while (n != 0)
	{
		++it;
		--n;
	}
}


template <typename RandomAccessIterator>
void advance_imple (RandomAccessIterator &it, size_t n,
			random_access_iterator_tag)
{
	it += n;
}

template <typename T>
void advance (T& it, size_t n)
{
	advance_impl(it, n, typename iterator_traits<T>::category())
}



template <typename T>
void f(T);

template <>
void f<int> (int);


template <typename T>
void f(T);

void f(int);

разница между ними только в том что в первом случае вызывается вторая
функция а во втором генерится из первой

В первом случае одна перегрузка и две специализации

А во втором две перегрузки в каждой по одной специализации

Если после каждого типа приписать звездочку (передавать указатель), то первый вариант не скомпилится,
А во втором случае вызовется f(int*)

В первом случае даже не понятно какая перегрузка. Сначала выбирается перегрузка!!! А потом специализация.





































