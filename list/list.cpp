#include "list.h"
#include <string>



list::iterator::iterator(node* a) :
	my(a) {};
std::string& list::iterator::operator*() {
	return my->val;
}
list::iterator& list::iterator::operator++() {
	my = my->next;
	return *this;
}
list::iterator& list::iterator::operator--() {
	my = my->prev;
	return *this;
}
list::iterator list::iterator::operator++(int) {
	return ++*this;
}
list::iterator list::iterator::operator--(int) {
	return --*this;
}
bool list::iterator::operator==(const iterator rhs) {
	return my == rhs.my;
}
bool list::iterator::operator!=(const iterator rhs) {
	return my != rhs.my;
}

list::const_iterator::const_iterator(node* a) :
	my(a) {};
std::string const& list::const_iterator::operator*() {
	return my->val;
}
list::const_iterator& list::const_iterator::operator++() {
	my = my->next;
	return *this;
}
list::const_iterator& list::const_iterator::operator--() {
	my = my->prev;
	return *this;
}
list::const_iterator list::const_iterator::operator++(int) {
	return ++*this;
}
list::const_iterator list::const_iterator::operator--(int) {
	return --*this;
}
bool list::const_iterator::operator==(const const_iterator rhs) {
	return my == rhs.my;
}
bool list::const_iterator::operator!=(const const_iterator rhs) {
	return my != rhs.my;
}

list::list() :
	node_fake(&node_fake, &node_fake, "") {}

list::list(list const& r) :
	node_fake(&node_fake, &node_fake, "") {
	for (const_iterator i = r.begin(); i != r.end(); i++) {
		push_back(*i);
	}
}

list& list::operator=(list const& r) {
	list t;
	for (const_iterator i = r.begin(); i != r.end(); i++) {
		t.push_back(*i);
	}
	empty();
	std::swap(t, *this);
	return *this;
}

list::~list() {
	clear();
}


void list::push_back(std::string const& s) {
	list::node* last = node_fake.prev;
	list::node* new_n = new list::node(last, &node_fake, s);
	node_fake.prev = new_n;
    last->next = new_n;
}

void list::pop_back() {
	if (empty())
		return;
	list::node* last = node_fake.prev->prev;
	delete last->next;
	last->next = &node_fake;
	node_fake.prev = last;
}

std::string const& list::back() const {
	return node_fake.prev->val;
}

std::string & list::back() {
	return node_fake.prev->val;
}

void list::push_front(std::string const& s) {
	list::node* first = node_fake.next;
	list::node* new_n = new list::node(&node_fake, first, s);
	node_fake.next = new_n;
	first->prev = new_n;
}

std::string const& list::front()const {
	return node_fake.next->val;
}

std::string& list::front() {
	return node_fake.next->val;
}

void list::pop_front() {
	if (empty())
		return;
	list::node* first = node_fake.next->next;
	delete first->prev;
	first->prev = &node_fake;
	node_fake.next = first;
}

bool list::empty() const {
	return node_fake.prev == &node_fake;
}

list::iterator list::begin() {
	return iterator(node_fake.next);
}

list::iterator list::end() {
	return iterator(&node_fake);
}

list::const_iterator list::begin() const {
	return const_iterator(node_fake.next);
}

list::const_iterator list::end() const{
	return const_iterator(node_fake.next->prev);
}

list::iterator list::erase(list::iterator pos) {
	node* a = pos.my->prev;
	node* b = pos.my->next;
	delete pos.my;
	a->next = b;
	b->prev = a;
	return iterator(b);
}

list::iterator list::insert(list::iterator pos, std::string const& val) {
	node* a = pos.my->prev;
	node* b = pos.my;
	node* new_n = new node(a, b, val);
	a->next = new_n;
	b->prev = new_n;
	return iterator(new_n);
}

void list::splice(list::iterator pos, list&, list::iterator first, list::iterator last) {
	node* a = pos.my->prev;
	node* b = pos.my;
	node* aa = first.my->prev;
	node* bb = last.my->next;
	aa->next = bb;
	bb->prev = aa;
	a->next = first.my;
	first.my->prev = a;
	b->prev = last.my;
	last.my->next = b;
}

void list::clear() {
	while (!empty())
		pop_back();
}
