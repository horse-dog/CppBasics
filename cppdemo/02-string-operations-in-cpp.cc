#include <iostream>
using namespace std;

void test_find() {
  string str = "hello";
  cout << str.find('l') << '\n';    // 2
  cout << str.find("el") << '\n';   // 1
  cout << str.find("olo") << '\n';  // max_ui64
  cout << string::npos << '\n';     // max_ui64
}

void test_rfind() {
  string str = "hello";
  cout << str.rfind('l') << '\n';   // 3
  cout << str.rfind("el") << '\n';  // 1
  cout << str.rfind("olo") << '\n'; // max_ui64
  cout << string::npos << '\n';     // max_ui64
}

int main(int argc, char* argv[]) {
  test_find();
  test_rfind();
  return 0;
}