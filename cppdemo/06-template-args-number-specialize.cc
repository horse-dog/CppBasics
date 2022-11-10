#include <iostream>
using namespace std;

template <typename T, typename U>
struct A {
  A() { cout << "ordinary version" << endl; }
  ~A(){};
};

template <>
struct A<int, double> {
  A() { cout << "int, double version" << endl; }
  ~A(){};
};

template <typename T>
struct A<T, double> {
  A() { cout << "T, double version" << endl; }
  ~A(){};
};

template <typename T, typename U>
void func(T a, U b) {
  cout << "ordinary version" << endl;
}

template <>
void func<>(int a, double b) {
  cout << "int, double version" << endl;
}

void test_template_args_number_specialize() {
  A<char, char> a;
  A<int, double> b;
  A<char, double> c;
}

void test_func_template_specialize() {
  func('a', 'b');
  func(3, 3.14);
}

int main(int argc, char *argv[]) {
  test_template_args_number_specialize();
  test_func_template_specialize();
  return 0;
}