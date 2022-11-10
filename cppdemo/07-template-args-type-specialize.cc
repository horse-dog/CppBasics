#include <iostream>
using namespace std;

template <typename T>
struct A {
  A() { cout << "ordinary version" << endl; }
  ~A(){};
};

template <typename T>
struct A<const T> {
  A() { cout << "const version" << endl; }
  ~A(){};
};

template <typename T>
struct A<T *> {
  A() { cout << "pointer version" << endl; }
  ~A(){};
};

template <typename T>
struct A<T &> {
  A() { cout << "left reference version" << endl; }
  ~A(){};
};

template <typename T>
struct A<T &&> {
  A() { cout << "right reference version" << endl; }
  ~A(){};
};

void test_template_args_type_specialize() {
  A<int> a;          // ordinary version
  A<const int> b;    // const version
  A<int *> c;        // pointer version
  A<const int *> d;  // pointer version
  A<int *const> e;   // const version
  A<int &> f;        // left reference version
  A<int &&> g;       // right reference version
}

int main(int argc, char *argv[]) {
  test_template_args_type_specialize();
  return 0;
}