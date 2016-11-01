struct game_object
{
	virtual void update(double dt) = 0; //документация, называем переменную чтобы понятно что это
	virtual void render() = 0;
};

struct static_object : virtual game_object
{
	void update(double) //не обозначаем переменную так как иначе будет unused variable
	{}
};

struct const_velocity_object : virtual game_object
{
	float x, y, dx, dy;
	void update (double dt) 
	{
		x += dx * dt;
		y += dy * dt;
	}
};

struct billboard_object : virtual game_object
{
	void render();
};

struct model_object : virtual game_object
{
	void render();
};

struct projectile : const_velocity_object, billboard_object // если бы было не виртуальное наследование мы бы не могла наследовать эти два класса так как ни update ни render не были бы заоверрайжены
{
	
};

// std:: istream ostream -- ввод вывод. наследники ios_base. iostream наследник их обоих istream и ostream наследуются виртуально
// в памяти в projectile внутри содержатся const_v_o и billboard_o, а game_object прикручивается game_object. для того чтобы знать где этот g_o хранится
// создается таблица виртуальных классов. 