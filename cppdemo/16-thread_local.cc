#include <iostream>
#include <thread>
using namespace std;

struct A {
 public:
  A &test(const std::string &name) {
    thread_local int count = 0;
    ++count;
    cout << name << ": " << count << std::endl;
    return *this;
  }
};

void func(const string &name) {
  A a1, a2;
  a1.test(name).test(name);
  a2.test(name).test(name);
}

void test_thread_local() {
  thread t1(func, "t1");
  t1.join();
  thread t2(func, "t2");
  t2.join();
}

int main(int argc, char *argv[]) {
  test_thread_local();
  return 0;
}