#include <iostream>
using namespace std;

int a = 1;
int &&b = 1;

void PrintV(int &t) { cout << "lvalue" << endl; }
void PrintV(int &&t) { cout << "rvalue" << endl; }

template <typename T>
void Test(T &&t) {
  PrintV(t);
  PrintV(std::forward<T>(t));
  PrintV(std::move(t));
}

int test_perfect_forward() {
  Test(1);                        // lvalue rvalue rvalue
  Test(a);                        // lvalue lvalue rvalue
  Test(b);                        // lvalue lvalue rvalue
  Test(std::forward<int>(1));     // lvalue rvalue rvalue
  Test(std::forward<int>(a));     // lvalue rvalue rvalue
  Test(std::forward<int &>(a));   // lvalue lvalue rvalue
  Test(std::forward<int &&>(a));  // lvalue rvalue rvalue
  return 0;
}

int main(int argc, char *argv[]) {
  test_perfect_forward();
  return 0;
}