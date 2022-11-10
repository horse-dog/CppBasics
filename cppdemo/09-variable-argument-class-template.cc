#include <iostream>
using namespace std;

template <typename... Args>
class A {};

template <>
class A<> {
 public:
  A() { cout << "A<>::A() called..." << endl; }
};

template <typename _First, typename... _Others>
class A<_First, _Others...> : private A<_Others...> {
 public:
  A(_First first, _Others... others) : m_i(first), A<_Others...>(others...) {
    cout << "A::A(first, others) called..., m_i = " << m_i << endl;
  }
  _First m_i;
};

template <typename... Args>
class B {};

template <>
class B<> {
 public:
  B() { cout << "B<>::B() called..." << endl; }
};

template <typename _First, typename... _Others>
class B<_First, _Others...> {
 public:
  B(_First first, _Others... others) : m_i(first), m_o(others...) {
    cout << "B::B(first, others) called..., m_i = " << m_i << endl;
  }
  _First m_i;
  B<_Others...> m_o;
};

void test_use_recursive_inherit() { A<int, float, double> myc(1, 2.1f, 3.14); }
void test_use_recursive_combinate() {
  B<int, float, double> myc(1, 2.1f, 3.14);
}

int main() {
  test_use_recursive_inherit();
  test_use_recursive_combinate();
  return 0;
}