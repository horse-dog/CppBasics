#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

once_flag flag;

void func() {
  call_once(flag, [] { cout << "call once" << endl; });
}

void test_call_once() {
  thread t1(func), t2(func);
  t1.join();
  t2.join();
}

int main(int argc, char *argv[]) {
  test_call_once();
  return 0;
}