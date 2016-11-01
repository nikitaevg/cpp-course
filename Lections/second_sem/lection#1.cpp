#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <assert.h>

using namespace std;

const int MAXN = 120;
typedef long long LL;


struct game_object_observer {
    void on_moved();
    void on_died();
};

struct game_object {
protected:
    int a = 4;
public:
    virtual int getA() {
        return a;
    }
    void subscribe(game_object_observer*);
    void unsubscribe(game_object_observer*);
};


struct mob : game_object, private game_object_observer { //game_o_o наследование может быть private и должен быть это значит, что только
    int getA() {                                         // в самом mob'e можно кастовать mob'a к game_o_o а также что нельзя вызывать track
        return a + 2;                                    // (который вообще public)
    }
private:
    void track(game_object* obj) {
        assert(!tracked);
        obj->subscribe(this); // тут this (mob) кастуется к game_object_observer
        tracked = obj;
    }
    void untrack() {
        tracked->unsubscribe(this); // тут тоже
        tracked = nullptr;
    }
    game_object* tracked;
};


// вынесем track в отдельный класс

struct game_object_tracker : game_object_observer {
    void track(game_object* target);
    void untrack();

private:
    game_object* tracked;
};

// private позволяет обращаться ко всем объектам из данного класса а protected позволяет то же и + в наследниках this->объект

// теперь mob:
//struct mob : game_object, private game_object_tracker {
//    void on_moved();
//    void on_died();
//};


// статический и динамический тип
// derived d;
// base2* b2 = &d;
// *base2 -- статический тип (который указан в коде) *derived -- динамический тип (который реальный)

//derived2 d2 вызывает конструктор derived2 потом derived потом base
//то же с delete. 

struct base {
    virtual ~base() {};
    virtual void print() = 0; // (=0) abstract функция
};

struct derived : base {
    derived(string const& msga) : msg(msga); // <==> this.msg = msga;
    void print() {
        cout << msg << '\n';
    }
private:
    string msg;
};

void func() {
    base *b = new derived("HHhhhhhhhhHHhhhhhhhhHHhhhhhhhhHHhhhhhhhhHHhhhhhhhhHHhhhhhhhhHHhhhhhhhhHHhhhhhhhhHHhhhhhhhhHHhhhhhhhhHHhhhhhhhhHHhhhhhhhh");
    b->print();
    delete b; //мы не удалим весь b так как msg не весь удалится. это из-за устройства string'a
}

struct base {
private:
    int a;
public:
    int b;
    void f() {
        a = 5;
        pair <int, int> b = {1, 2};
    }
};

struct derived:base {
    int b, c;
};

void t1() {
    derived d;
    base b = d;   // <==> base b((base &)d); вызов окнструктора "от себя" с кастованием
    b.f();
}

void t2() {
    derived d;
    base& b = d;
    b.f();
}

void t3() {
    derived d;
    base* b = &d;       // b и d указывают в одно и то же место
    b->f();
}

int main() {
    mob m;
    game_object &o = m;
    cout << o.getA();
    //game_object g = game_object();
    //cout << m.getA() << g.getA();
}