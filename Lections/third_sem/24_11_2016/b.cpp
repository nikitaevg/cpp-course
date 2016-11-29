Дз:
свой bind

int f(int, int, int);

bind(&f, 1, 2, 3)(); //f(1, 2, 3)
bind(&f, 4, _1, 5)(6); //f(4, 5, 6)
bind(&f, _1, _2, _1)(7, 8); //f(7, 8, 7);

bind(&f, bind(&g, _1, _2), _1, _2)(9, 10); // f(g(9, 10), 9, 10);

Вернемся к function
Такую обертку можно придумать для разных классов с одним интерфейсом
Есть такая-же хрень
any_iterator<int, forward_iterator> это полиморфные врапперы (или какие-то такие)
Или type erasur-ы

Есть такая штука any -- обертка, которая подходит для всех классов.
any_cast -- каст к какому-то классу.

Обсудим signal

signal<int, int> s; добавляем или удаляем функции, и когда вызвали сигнал, все эти функции будут вызваны

connection c1 = s.connect([](int a, int b){...});
s(1, 2);
connection c2 = s.connect([](int a, int b){...});
s(3, 4); -- вызовутся c1 и c2 (все кто подписан)

c1.disconnect();
s(5, 6);
c2.disconnect();
s(7, 8);

реализация сигнала

template <typename...Params>
struct signal
{
	typedef std::function<void (Params...)> slot_t;
	typedef size_t id_t;

	connection connect(slot_t slot)
	{
		id_t = next++;
		slots_insert({id, std::move(slot)});
		return connection(id);
	}

	void operator(Params...p) const
	{
		for (auto i = slots.cbegin(); i!= slots.cend(); i++)
			i->second(p...);
	}

	struct connection
	{
		connection(signal sig, id_t id)
		:signal(sig), id(id)
		{}
		void disconnect()
		{
			sig->slots.erase(slots.find(id));
		}
	private:
		signal = sig;
		id_t id;
	}

private:
	id_t next;
	std::map<id_t, slot_t> slots;


}

реализация () будет ломаться, когда мы захотим отписаться в функции, так как итератор i инвалидируется.
Если просто копировать мапу, то можно будет вызвать уже удаленные функции, пока не закончится цикл


Можно вместо мапы хранить лист

template <typename...Params>
struct signal
{
	typedef std::function<void (Params...)> slot_t;
	typedef size_t id_t;

	connection connect(slot_t slot)
	{
		slots_insert(std::move(slot));
		return connection(this, id);
	}

	void operator(Params...p) const
	{
		for (auto i = slots.cbegin(); i!= slots.cend(); i++)
			if (i->first)
				i->second(p...);
		for (auto i = slot.begin(); i!= slots.end(); i++)
			if (!i->first) 
				i = slots.erase(i);
			else
				++i;
	}

	struct connection
	{
		connection(Iterator it, signal sig)
		:signal(sig),
		id(it)
		{}
		void disconnect()
		{
			i->first = false;
		}
	private:
		signal = sig;
		id_t id;
	}

	mutable std::list<std::pair<bool, slot_t>> slots;


}

Но и это может ломаться на таком тесте
connect c1
connect c2
operator()
	c1()
		operator()
			c1()
			c2()
		cleanup
	++i

Что-то будет не так -- итератор в цикле станет невалидным
Это называется reentrancy