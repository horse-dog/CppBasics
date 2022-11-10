#include <thread>
#include <future>
#include <iostream>
using namespace std;

void func(std::future<int>& fut) {
  int x = fut.get();
  cout << "value: " << x << endl;
}

void test_promise() {
  std::promise<int> prom;
  std::future<int> fut = prom.get_future();
  std::thread t(func, std::ref(fut));
  this_thread::sleep_for(5s);
  prom.set_value(6);
  t.join();
}

int main(int argc, char* argv[]) {
  test_promise();
  return 0;
}