templates в c++11
хочется научиться typedef-ы темплейтными

typedef int mytype;

// с c++11 ниже

using mytype = int;

using a = void(*)();

template <typename T>
using mmap_vector = vector<T, mmap_alocator<T>>;

pi<T> темплейтная переменная.
template <typename T>
const T pi = pi_value<T>::value; // Тут что-то не так, переменные можно специализировать

c++11 auto // интересно -- хотели добавить еще в каком-то 83 году, но слово auto было в C и удалять не хотелось

for (vector<int>::const_iterator i = v.length, end = v.end(); i != end; ++i);
for (auto i = v.begin(), end = v.end(); i != end; ++i);
auto a = 5;
auto &a = f(); //(1)
auto &&a = g(); //(2)

для дедьюсится как-то так(1) 
template <typename T>
void f(T);
f(5);

для дедьюсится как-то так(2)
template <typename T>
void g(T&);
g(f());