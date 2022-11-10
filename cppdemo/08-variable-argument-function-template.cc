#include <iostream>
using namespace std;

void func() { cout << "receive end..." << endl; }

template <typename T, typename... U>
void func(const T &firstArg, const U &...otherArgs) {
  cout << "received: " << firstArg << endl;
  func(otherArgs...);
}

template <class Callable, class... Args>
void pfunc(Callable &&fn, Args &&...args) {
  cout << disjunction_v<is_arithmetic<Args>...> << endl;
  fn(forward<Args>(args)...);
}

void test_variable_args_func_template() { func(1, 1.1, "hello"); }

void test_varibale_args_used_in_func() {
  pfunc(printf, "there're %d, %lf, %s\n", 1, 1.2, "cplusplus");
}

int main() {
  test_variable_args_func_template();
  test_varibale_args_used_in_func();
  return 0;
}