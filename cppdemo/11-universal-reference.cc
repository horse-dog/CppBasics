#include <boost/type_index.hpp>
#include <iostream>
using namespace std;

template <typename T>
void func_t(T val) {
  cout << "*****************************" << endl;
  using boost::typeindex::type_id_with_cvr;
  cout << "T   = " << type_id_with_cvr<T>().pretty_name() << endl;
  cout << "val = " << type_id_with_cvr<decltype(val)>().pretty_name() << endl;
  cout << "*****************************" << endl;
}

template <typename T>
void func_tr(T &val) {
  cout << "*****************************" << endl;
  using boost::typeindex::type_id_with_cvr;
  cout << "T   = " << type_id_with_cvr<T>().pretty_name() << endl;
  cout << "val = " << type_id_with_cvr<decltype(val)>().pretty_name() << endl;
  cout << "*****************************" << endl;
}

template <typename T>
void func_ctr(const T &val) {
  cout << "*****************************" << endl;
  using boost::typeindex::type_id_with_cvr;
  cout << "T   = " << type_id_with_cvr<T>().pretty_name() << endl;
  cout << "val = " << type_id_with_cvr<decltype(val)>().pretty_name() << endl;
  cout << "*****************************" << endl;
}

template <typename T>
void func_trr(T &&val) {
  cout << "*****************************" << endl;
  using boost::typeindex::type_id_with_cvr;
  cout << "T   = " << type_id_with_cvr<T>().pretty_name() << endl;
  cout << "val = " << type_id_with_cvr<decltype(val)>().pretty_name() << endl;
  cout << "*****************************" << endl;
}

int integer = 10;
int &int_ref = integer;
const int const_int = integer;
const int &const_int_ref = integer;
int int_arr[10];
int *int_ptr;
int &&int_rref = 100;

void test_func_t() {
  func_t(integer);        // T: int, val: int
  func_t(int_ref);        // T: int, val: int
  func_t(const_int);      // T: int, val: int
  func_t(const_int_ref);  // T: int, val: int
  func_t(int_arr);        // T: int*, val: int*
  func_t(int_ptr);        // T: int*, val: int*
  func_t(100);            // T: int, val: int
  func_t(int_rref);       // T: int, val: int
}

void test_func_tr() {
  func_tr(integer);        // T: int, val: int &
  func_tr(int_ref);        // T: int, val: int &
  func_tr(const_int);      // T: int const, val: int const&
  func_tr(const_int_ref);  // T: int const, val: int const&
  func_tr(int_arr);        // T: int [10], val: int (&)[10]
  func_tr(int_ptr);        // T: int*, val: int* &
  // func_tr(100);         // compile error
  func_tr(int_rref);       // T: int, val: int &
}

void test_func_ctr() {
  func_ctr(integer);        // T: int, val: int const&
  func_ctr(int_ref);        // T: int, val: int const&
  func_ctr(const_int);      // T: int, val: int const&
  func_ctr(const_int_ref);  // T: int, val: int const&
  func_ctr(int_arr);        // T: int [10], val: int const (&) [10]
  func_ctr(int_ptr);        // T: int*, val: int* const&
  func_ctr(100);            // T: int, val: int const&
  func_ctr(int_rref);       // T: int, val: int const&
}

void test_func_trr() {
  func_trr(integer);        // T: int &, val: int &
  func_trr(int_ref);        // T: int &, val: int &
  func_trr(const_int);      // T: int const &, val: int const &
  func_trr(const_int_ref);  // T: int const &, val: int const &
  func_trr(int_arr);        // T: int (&) [10], val: int (&) [10]
  func_trr(int_ptr);        // T: int* &, val: int* &
  func_trr(100);            // T: int, val: int &&
  func_trr(int_rref);       // T: int &, val: int &
}

int main() {
  test_func_t();
  test_func_tr();
  test_func_ctr();
  test_func_trr();
  return 0;
}