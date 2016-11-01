Реализации std::string:

В Microsoft

struct string
{
	T* data;
	size_t size; -- сколько символов в строке
	size_t capacity; -- сколько может поместиться
}

struct string
{
	size_t size;
	size_t capacity;
	union
	{
		T* data;
		T static_data[16];
	}
}

В gcc какой-то версии capacity засунули вместе с static-data. В string от microsoft были продолбы в 8 байт на capacity в отличие от gcc

У llvm:
первый бит 1 если большая строка, 0 если небольшая.
У большой потом идет capacity 8 байт потом size потом data (8-8-8)
У маленькой сначала 1 байт size а потом 23 -- строка. 1-23

Чтобы написать это надо использовать юнион двух структур.
struct large_string_buf
{
	size_t capacity;
	size_t size;
	char* data;
}
struct small_string_buf
{
	uint8_t size;
	char data[23];
}


union string {
	large_string_buf large;
	small_string_buf small;
}

struct header
{
	size_t size, capacity, refs;
};

struct string
{
	char *data;
	header* get_header() {
		return (header*) data - 1;
	}

	void alloc_data(size_t size)
	{
		header* ptr = new header(sizeof(header)+size);
		data = (char*)(ptr + 1);
	}
};













































