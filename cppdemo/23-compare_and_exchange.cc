#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

using namespace std;

struct Node {
  int value;
  Node* next;
};

std::atomic<Node*> list_head(nullptr);

void append(int val) {
  // head insert
  Node* newNode = new Node{val, list_head};
  while (!(list_head.compare_exchange_weak(newNode->next, newNode)))
    ;
}

void test_compare_and_exchange() {
  // spawn 10 threads to fill the linked list:
  std::vector<std::thread> threads;
  for (int i = 0; i < 10; ++i) threads.push_back(std::thread(append, i));
  for (auto& th : threads) th.join();

  // print contents:
  for (Node* it = list_head; it != nullptr; it = it->next)
    std::cout << ' ' << it->value;

  std::cout << '\n';

  // cleanup:
  Node* it;
  while ((it = list_head) != nullptr) {
    list_head = it->next;
    delete it;
  }
}

int main(int argc, char* argv[]) {
  test_compare_and_exchange();
  return 0;
}