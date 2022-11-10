#include <thread>
#include <future>
#include <iostream>
using namespace std;

int func(int in) {
  this_thread::sleep_for(5s);
  return in + 1;
}

int main() {
  auto fut = async(func, 5);
  cout << fut.get() << endl;
  return 0;
}