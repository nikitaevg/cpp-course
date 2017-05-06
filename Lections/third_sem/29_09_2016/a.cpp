Примеры чего-то

Пусть функция принимает n переменных и ищет минимальный

Если ей передали 0 элементов, можно: UF, throw exception, вернуть минимальный элемент типа T
Функциия не будет работать если тип нельзя копировать если сравнивать

template <typename T>
T min_element (T const* data, size_t size)
{
	T result = max_value<T>::result;
	for (size_t i = 0; i != size; ++i)
		result = min(result, data[i]);
	return result;
}

Что писать в самом начале в result?
Создадим 
template <typename T>
struct maxvalue;

template <>
struct max_value <int>
{
	static int const result = 0x7fffffff;
};
это мета-функция (вычисляется в compiled time) или это trait для типа T

Напишем функцию проверяющую приведет ли к переполнению умножение

template <typename Signed>
bool signed_mul_overflow (Signed a, Signed b)
{
	typedef typename twice <Signed>::type twice_t;
	twice_t c = twice_t(a) * twice_t(b);
	return c < numeric_limits<Signed>::min() || c > numeric_limits<Signed>::max();
}

Сортировка

template <typename T>
void sort (T& data, size_t size, bool (*less)(T const&, T const&)); //(1)

template <typename T, typename C>  //(2)
void sort(T* data, size_t size, C less) {
	less (data[i], data[j]);
}


struct less
{
	bool operator ()(T a, T b) const
	{
		return a < b;
	}
}

sort (a, b, less<int>());

(1) или (2)?
+ (1) -- single copy of binary code, так как для темплейтов создается куча кода. Также возможно выбирать компаратор в рантайм. Допускает раздельную
			компиляцию
+ (2) -- Компилятор сможет оптимизировать код, в первом случае не сможет, так как в первом случае передается указатель на функцию, который не 
			оптимизируется. А во втором это не виртуальная функция и она подставляется и оптимизируется. 
			В случае сортировки большого массива выигрыш составляет 2.5 раза. Круто. Также 


Первый случай это runtime(динамический) template, а второй
			compile time template.
































