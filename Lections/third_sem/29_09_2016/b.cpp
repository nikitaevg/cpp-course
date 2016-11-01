Бимап:

template <typename Left, typename Right>
struct bimap
{
	template <typename tag>
	struct iterator
	{
		node* current;
		typedef value_type; // задача -- сделать value_type либо Left либо Right в зависимости от tag
		typedef typename tag_traits <Left, Right, Tag>::value_type result;
		value_type const& operator* () const;
		{
			return current-> (хотим вызывать разные мемберы в зависимости от тега)
			return tag_traits<Left, Right, TAg>::get_value(current);
		}
	};
	typedef iterator <left_tag> left_iterator;
	typedef iterator <right_tag> right_iterator;
};

template <typename L, typename R, typename Tag>
struct tag_traits;
template <typename L, typename R>
struct tag_traits<L, R, left_tag> {
	typedef L result;
	static value_type& get_value(node* n)
	{
		return n->left.data;
	}
}

Если хотим пошарить код между двумя (или больше) типами, можно параметризовать это и задать tag_traits

Сверху избыточный код, так как ты таскали за собой три типа, хотя по сути нужен один. 
<int, double, left_tag> <int, float, left_tag> -- должны генерировать один и тот же код, а будут разные. 
<int, double, left_tag> <float, int, right_tag> -- тоже, хотя тоже должны бы
Можно улучшить, уменьшить количество генерируемого кода.

template <typename Data>
struct half_node
{
	Data data;
	half_node* left, *right, *parent;
};

template <typename Left, typename Right>
struct node
{
	half_node<Left> left;
	half_node<Right> right;
}

С халф-нод будет передаваться один тип и для одного типа будет создаваться только один код.

#define container_of( ptr, type, member ) \
   ( \
      { \
         const decltype( ((type *)0)->member ) *__mptr = (ptr); \
         (type *)( (char *)__mptr - offsetof( type, member ) ); \
      } \
   )

half_node <int> * p;
node* q = container_of(p, node, left);

Также это можно реализовать с помощью наследования:
left_node и right_node наследуются от node. 
Чтобы node->right_node надо скастить node к right_node
	  right_node->node надо скастить обратно

template <typename Data>
struct half_node
{
	Data data;
	half_node* left, *right, *parent;
};

template <typename Left, typename Right>
struct node : half_node<Left>, half_node<Right> -- сломается если Right==Left
{
};

struct base {
	int a;
}

struct derived : base {
	int b;
}

int main() {
 derived d1;
 derived* d2 = &d1;
 base* b3 = d2;
 derived* d4 = (derived*) b3;
 assert(d2 == d4);

 derived d1;
 derived* d2 = &d1;
 base* b3 = b2;
 base b4 = *b3;
 derived* d5 = (derived*)&b4;
 assert (d5 != d2);
}


























