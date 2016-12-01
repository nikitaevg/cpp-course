//flashback
decltype(auto) x = f(); <==> decltype(f()) x = f();
эквивалентно
int x;
int& x;

auto&& x = f();
эквивалентно
int&& x;
int& x;

//-----------------------------------------------------

Начальные значения (последнее время события, если события не было)
std::optional<T>
optional (T);
operator=(T);
optional();
operator bool() const; -- optional<int> a; if (a) -- тут будет вызван operator bool
T& operator*(); // UB if !valid
T& value(); // throws if !valid
bool valid
? data; // valid <==> data - initialized Если написать T data, то будет вызван пустой конструктор

Тут минус в том, что хранится лишний bool и тратится память
Можно заменить optional на T*, но это хуже, так как придется постоянно аллоцировать память
T value_or(T const& default_value) const; 

Есть штука как tribool у нее есть состояния true, false, indeterminated

template <typename... Args>
void emplace(Args&&... args); // basic throw warranty. Для случаев если T не movable. Если бросил конструктор, то valid = false;

tamplate <typename... Args>
optional(in_place_t, Args&&... args);
e.g. :
optional<mytype> a(in_place_t, 1, 2, 3); создает optional с mytype от таких аргументов
nullopt -- ресетит 
a = nullopt; возвращает к invalid значению.

Что писать вместо ? data;?
//union от типов с конструкторами и деструкторами можно делать только с c++17
до c++11
bool valid;
char data[sizeof(T)];

Чтобы вызвать конструктор
new (&data)T(...)

Но может не сработать. 
Данные надо хранить так -- если n-байтный тип, то хранить его в адресе кратном n
В x86 это не обязательно, но желательно. Если будет невыравнено, то обращение к памяти долго.
В этой массив char-ов будет выравниваться по чарам (по 1 байту), а значит обращение к памяти будет долго.

Если такая структура легла на границе 64-байтных блоков, то будет вообще не работать. Будет два обращения к памяти, intel вроде отказывается с этим работать.

alignment структуры = max (alignment_members)

в с++11
появились alignof
alignas
int alignas(8) a; это аргумент объявления. Появился в c++11. 
Теперь можно так реализовывать
bool valid
std::aligned_storage<sizeof(T), alignof(T)>::type data;

reinterpret_cast<T*>(&data);
new (&data)T(...);
reinterpret_cast<T*>(&data)->~T();


//----------------------------------------------------------------------------------------

Обобщение optional -- variant

std::variant<int, string, vector<float> > -- как юнион, только правильно вызывает конструктор, деструктор
	int which
	? data; // with enough space for each type of data.

