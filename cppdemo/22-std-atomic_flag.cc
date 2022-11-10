#include <atomic>
#include <iostream>
#include <thread>
#include <vector>
using namespace std;

std::atomic_flag locker = ATOMIC_FLAG_INIT;

void func(int n) {
  for (int cnt = 0; cnt < 10; ++cnt) {
    while (locker.test_and_set(std::memory_order_acquire))  // acquire lock
      ;                                                     // spin
    std::cout << "Output from thread " << n << '\n';
    locker.clear(std::memory_order_release);  // release lock
  }
}

void test_atomic_flag() {
  std::vector<std::thread> v;
  for (int n = 0; n < 10; ++n) v.emplace_back(func, n);
  for (auto& t : v) t.join();
}

int main(int argc, char* argv[]) {
  test_atomic_flag();
  return 0;
}