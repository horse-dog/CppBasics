#include <atomic>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <cassert>

#include "utils/tick.h"

using namespace std;

class raw_spinlock {
 public:
  raw_spinlock() noexcept {};
  void lock() noexcept {
    while (flag.test_and_set(std::memory_order_acquire))
      ;
  }
  void unlock() noexcept { flag.clear(std::memory_order_release); }
  bool try_lock() noexcept {
    return !flag.test_and_set(std::memory_order_acquire);
  }

 private:
  std::atomic_flag flag = ATOMIC_FLAG_INIT;
};

#define M 10
#define N 100000

int counter1 = 0;
int counter2 = 0;
raw_spinlock spin_locker;
std::mutex mtx_locker;

int test_spinlock() {
  vector<thread> th;
  for (int i = 0; i < M; i++) {
    th.emplace_back([] {
      for (int i = 0; i < N; i++) {
        spin_locker.lock();
        counter1++;
        spin_locker.unlock();
      }
    });
  }
  for (int i = 0; i < M; i++) th[i].join();
  return counter1;
}

int test_mtxlock() {
  vector<thread> th;
  for (int i = 0; i < M; i++) {
    th.emplace_back([] {
      for (int i = 0; i < N; i++) {
        mtx_locker.lock();
        counter2++;
        mtx_locker.unlock();
      }
    });
  }
  for (int i = 0; i < M; i++) th[i].join();
  return counter2;
}

void test_lock() {
  assert(TICK(test_spinlock) == M * N);
  assert(TICK(test_mtxlock) == M * N);
}

int main(int argc, char *argv[]) {
  test_lock();
  return 0;
}