#include <iostream>
#include <memory>
using namespace std;

struct A;
struct B;
struct A {
  std::shared_ptr<B> bptr;
  ~A() { cout << "A delete" << endl; }
  void Print() { cout << "A" << endl; }
};
struct B {
  std::weak_ptr<A> aptr;
  ~B() { cout << "B delete" << endl; }
  void PrintA() {
    if (!aptr.expired()) {
      auto ptr = aptr.lock();
      ptr->Print();
    }
  }
};

void test_circular_reference() {
  auto aaptr = std::make_shared<A>();
  auto bbptr = std::make_shared<B>();
  aaptr->bptr = bbptr;
  bbptr->aptr = aaptr;
  bbptr->PrintA();
}

int main(int argc, char *argv[]) {
  test_circular_reference();
  return 0;
}