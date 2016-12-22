std::condition_variable

struct concurrent_queue {
	void push(T data) {
		std::lock_guard<std::mutex> lg(m);
		bool was_empty = q.empty();
		q.push_back(std::move(data));
		lg.unlock();
		if (was_empty)
			cv.notify_one(); // в таком варианте если быстро пушить, то в первом пуше очередь будет ноль, а в остальных не ноль, так как не успеет попнуться
				// короче не вариант, и надо без was_empty писать. Но это может разбудить много pop-ов. Без ифа проблема thundering herd
	}

	std::optional<T> try_pop() {
		std::lock_guard<std::mutex> lg(m);
		if (q.empty())
			return nullopt;
		std::optional<T> result(q.front());
		q.pop_front();
		return result;
	}

	T pop() {
		Если один поток попает, а другой пушит, надо подождать пока второй допушит. Это можно реализовать с помощью std::condition_variable
		std::unique_lock<std::mutex> lg(m);
		while (q.empty()) {
			lg.unlock();
			cv.wait(); // эта функция принимает лок, который надо отпустить
			lg.lock();
			// эта реализация сломается если мы анлочим лок, в это же время запушим и нотифайнем. Это все сломает. По этому сделаем так
			cv.wait(lg);
		}
		optional<T>result = q.fromt();
		q.pop_front();
		return result;
	}

private:
	std::deque<T> q;
	std::mutex m;
	std::condition_variable cv;
}