#include <iostream>
#include <memory>
#include <tuple>
using namespace std;

namespace ptr {
template <typename U>
struct Deletor {
  void operator()(U* p) { delete p; }
};

template <typename T, typename U = Deletor<T>>
class unique_ptr {
 private:
  T*& _Ptr() { return std::get<0>(m_tu); }
  T* _Ptr() const { return std::get<0>(m_tu); }
  U& _Del() { return std::get<1>(m_tu); }

 public:
  explicit unique_ptr() { _Ptr() = nullptr; };
  explicit unique_ptr(T* p) { _Ptr() = p; };

  unique_ptr(unique_ptr<T, U>&& p) {
    this->_Ptr() = p._Ptr();
    p._Ptr() = nullptr;
  };

  unique_ptr<T, U>& operator=(unique_ptr<T, U>&& p) {
    if (&p == this) return *this;
    reset();
    new (this) unique_ptr(std::move(p));
    return *this;
  };

  ~unique_ptr() {
    if (_Ptr()) _Del()(_Ptr());
    _Ptr() = nullptr;
  };

  unique_ptr(const unique_ptr<T, U>& p) = delete;
  unique_ptr<T, U>& operator=(const unique_ptr<T, U>& p) = delete;
  unique_ptr<T, U>& operator=(T* p) = delete;

  T* release() {
    T* p = _Ptr();
    _Ptr() = nullptr;
    return p;
  };

  void reset(T* p = nullptr) noexcept {
    if (_Ptr()) _Del()(_Ptr());
    _Ptr() = p;
  };

  void swap(unique_ptr<T, U>& rhs) noexcept { std::swap(m_tu, rhs.m_tu); };

  T* get() noexcept { return _Ptr(); };

  U& get_deletor() noexcept { return _Del(); };

  explicit operator bool() const noexcept { return _Ptr() != nullptr; };

  T& operator*() const noexcept { return *_Ptr(); };

  T* operator->() const noexcept { return _Ptr(); };

 private:
  tuple<T*, U> m_tu;
};
}  // namespace ptr

void test_my_unique_ptr() {
  cout << sizeof(std::unique_ptr<int>) << endl;
  cout << sizeof(ptr::unique_ptr<int>) << endl;
  ptr::unique_ptr<int> a(new int(100));
  ptr::unique_ptr<int> b(new int(200));
  cout << *a << endl;
  cout << *b << endl;
  std::swap(a, b);
  cout << *a << endl;
  cout << *b << endl;
}

int main(int argc, char* argv[]) {
  test_my_unique_ptr();
  return 0;
}