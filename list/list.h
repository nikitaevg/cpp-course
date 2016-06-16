#include <string>

#ifndef LIST_LIST_H
#define LIST_LIST_H

struct list {
	list ();
	list (list const&);
	~list();
	list & operator=(list const&);
	void push_back(std::string const&);
	void pop_back();
	std::string const& back() const;
	std::string & back();
	void push_front(std::string const&);
	std::string const& front()const;
	std::string& front();
	void pop_front();
	bool empty() const;
	struct iterator;
	struct const_iterator;
	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;
	iterator erase(iterator pos);
	iterator insert(iterator pos, std::string const& val);
	void splice(iterator pos, list&, iterator first, iterator last);
	void clear();
private:
	struct node {
		node(node* pre, node* nex, std::string va) : 
			prev(pre),
			next(nex),
			val(va){
		}
		node *next, *prev;
		std::string val;
	};
	node node_fake;
};

struct list::iterator {
	node* my;
	iterator(node *a);
	std::string& operator*();
	list::iterator& operator++();
	list::iterator& operator--();
	list::iterator operator++(int);
	list::iterator operator--(int);
	bool operator==(const iterator rhs);
	bool operator!=(const iterator rhs);
};
struct list::const_iterator {
	node* my;
	const_iterator(node *a);
	std::string const& operator*();
	list::const_iterator& operator++();
	list::const_iterator& operator--();
	list::const_iterator operator++(int);
	list::const_iterator operator--(int);
	bool operator==(const const_iterator rhs);
	bool operator!=(const const_iterator rhs);
};



#endif //LIST_LIST_H
