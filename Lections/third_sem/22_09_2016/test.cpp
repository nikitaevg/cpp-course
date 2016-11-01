// template <typename X, typename Y>
// void f(X x, Y y) {

// }

// template <typename A (*)(B), typename B (*)(C)>
// void f(A a, B b) {

// }

template <typename U, typename V>
void f(U, V);

template <typename A, typename B, typename C, typename D>
void f(A(*)(B), C(*)(D));