#include <iostream>
#include <vector>
using namespace std;

template <typename T>
using myvector = vector<T, allocator<T>>;

template <typename T, template <typename> typename Container>
class A {
  Container<T> c;

 public:
  void push_back(const T &val) { c.push_back(val); }
  void print() const {
    for (auto &item : c) {
      cout << item << ' ';
    }
    cout << endl;
  }
};

void test_template_template_args() {
  A<int, myvector> ivc;
  ivc.push_back(3);
  ivc.push_back(4);
  ivc.push_back(5);
  ivc.print();
}

int main() {
  test_template_template_args();
  return 0;
}