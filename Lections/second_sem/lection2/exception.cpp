struct x
{};
struct y
{};

void g()
{
	y yy1;
	if(true)
		throw x();
}

void f()
{
	y yy2;
	g();
}

void main() {
	try
	{
		f();
	}
	catch (x const& e)
	{

	}
}