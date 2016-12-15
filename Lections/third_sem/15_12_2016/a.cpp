Multithreading

До многоядерных процессоров
ускорение выполнения программ
out-of-order execution
SIMD
Многопоточность была и раньше и реализовалась с помощью прерываний
Можно также несколько компьютеров использовать с одной расшареной памятью, 
или без шареной памяти (concurrent и distributed соответственно)
Программа может не выигрывать от распараллелирования, так как у процессор, 
диск и оперативная память в одном экземпляре и все может упираться в их ресурс
Привет Скакову

std::thread th([]()
{
	...
});
th.join() -- дождаться выполнения потока
th.detach() -- какая-то функция, до конца не понял. Функция нужна очень редко, в очень паталогических случаях

поток начинает выполнение сразу после объявления, если в него передана функция
Также можно создать с помощью пустого конструктора, а потом запустить уже на функции
Это легковесная обертка, у нее небольшой интерфейс

Проблемы многопоточности:
std::array<int, 10000> accounts;

void transfer(size_t from, size_t to, int account) 
{
	if (accounts[from] < amount)
		thrwo insufficient_fund();
	accounts[from] -= amount;
	accounts[to] += amount;
}

// проблемы в том, что можем списать одновременно x денег с одного аккаунта и у нас может стать меньше 0 денег.
// В разных потоках проерка прошла без проблем. Также если две операции на одном аккаунте, мы можем перекрыть 
// запись в accounts[to] так как там три атомарные операции.

Mutexes

std::mutex m;

void transfer(size_t from, size_t to, int account) 
{
	std::lock_guard<std::mutex> lg(m); // lock_guard -- умная оболочка для мьютекса. Работает с разными мьютексами (их есть несколько)
	if (accounts[from] < amount)
		throw insufficient_fund();
	accounts[from] -= amount;
	accounts[to] += amount;
}

Закон Амдала
Пусть у нас есть alpha кода - должна выполняться последовательно,
а 1 - alpha может выполняться параллельно
и N - количество исполнителей (потоков)

alpha = s
p = 1 - alpha
N == 1 | s  |   p   |
N == 2 | s  | p/2|
N == 3 | s  |p/3|

1/(alpha + (1 - alpha) / N)

struct account {
	int money;
	std::mutex m;
}

void transfer(size_t from, size_t to, int account) 
{
	std::lock_guard<std::mutex> lg(accounts[from].m);
	std::lock<std::mutex> lg2(accounts[to].m);
	if (accounts[from].money < amount)
		throw insufficient_fund();
	accounts[from].money -= amount;
	accounts[to].money += amount;
}

Теперь при переводе со счета x на y и с a на b то мьютекса не будет, так как теперь на каждый аккаунт свой мьютекс
Либо если переводим со счета a на a, undef

Но теперь при переводе с a на b и b на a одновременно в первой строчке оба потока залочат мьютексы обратного перевода
И оба будут ждать бесконечно пока во второй строке не разлочится мьютекс

Это как-то связано с состоянием гонки. 
Пример -- запустили два раза одновременно transfer и непонятно какой из них выполнится первым. Второй может и не выполниться
состояние гонки -- ситуация, когда результат выполнения зависит от того, в каком порядке выполнятся потоки.

Проблема, описанная выше -- deadlock
Чтобы ее решить можно сначала лочить минимум, потом максимум.

void transfer(size_t from, size_t to, int account) 
{
	if (from != to)
		std::lock_guard<std::mutex> lg(accounts[min(from, to)].m);
	std::lock<std::mutex> lg2(accounts[max(from, to)].m);
	if (accounts[from].money < amount)
		throw insufficient_fund();
	accounts[from].money -= amount;
	accounts[to].money += amount;
}

Можно нарисовать граф мьютексов, где вершины это мьютексы, а ребро из a в b есть если какой-то поток, удерживая мьютекс 
a может просить мьютекс b.

Если есть цикл в этом графе, то есть вероятность дедлока. Для возникновения дедлока требуется столько потоков, сколько ребер в цикле































