#include <functional>
#include <mutex>
#include <thread>
#include <condition_variable>
using namespace std;

#define MAX_SIZE 5

class Semaphore {
 public:
  Semaphore(int count = 0) : count(count){};

  void signal() {
    std::unique_lock<std::mutex> lock(mut);
    ++count;
    cv.notify_one();
  }

  void wait() {
    std::unique_lock<std::mutex> lock(mut);
    cv.wait(lock, [this] { return count > 0; });
    --count;
  }

 private:
  std::mutex mut;
  std::condition_variable cv;
  int count;
};

auto P = std::mem_fn(&Semaphore::wait);
auto V = std::mem_fn(&Semaphore::signal);

struct Item {
  unsigned int no;
  chrono::time_point<chrono::steady_clock> time;
};

// global semaphore
Semaphore q_empty = MAX_SIZE;
Semaphore q_full = 0;
std::mutex locker;

// global queue
Item qu[5];
int cur = 0;

// global counter
unsigned int counter = 0;

Item make_item() {
  this_thread::sleep_for(500ms);
  printf("make item[No.%u]\n", counter);
  return {counter++, chrono::steady_clock::now()};
}

void insert_item(const Item &item) {
  printf("insert item[No.%u]\n", item.no);
  qu[cur++] = item;
}

Item get_item() {
  auto item = qu[--cur];
  printf("get item[No.%u]\n", item.no);
  return item;
}

void consume_item(const Item &item) {
  this_thread::sleep_for(500ms);
  printf("consume item[No.%u]\n", item.no);
}

void produce_process() {
  while (true) {
    auto item = make_item();
    P(q_empty);
    locker.lock();
    insert_item(item);
    locker.unlock();
    V(q_full);
  }
}

void consume_process() {
  while (true) {
    P(q_full);
    locker.lock();
    auto item = get_item();
    locker.unlock();
    V(q_empty);
    consume_item(item);
  }
}

void test_condition_varibale() {
  thread producer(produce_process);
  thread consumer(consume_process);
  producer.join();
  consumer.join();
}

int main(int argc, char *argv[]) {
  test_condition_varibale();
  return 0;
}