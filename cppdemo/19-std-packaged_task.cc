#include <thread>
#include <future>
#include <iostream>
using namespace std;

int func(int in) {
  this_thread::sleep_for(5s);
  return in + 1;
}

void test_packaged_task() {
  std::packaged_task<int(int)> task(func);

  auto fut = task.get_future();
  std::thread(std::ref(task), 5).join();
  cout << "result: " << fut.get() << endl;

  task.reset();
  fut = task.get_future();
  std::thread(std::ref(task), 10).join();
  cout << "result: " << fut.get() << endl;
}

int main(int argc, char *argv[]) {
  test_packaged_task();
  return 0;
}