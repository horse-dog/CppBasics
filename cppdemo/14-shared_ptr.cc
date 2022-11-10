#include <memory>
#include <iostream>
using namespace std;

namespace ptr {
template <typename T>
class shared_ptr {
 private:
  size_t* m_count;
  T* m_ptr;

 public:
  shared_ptr() : m_ptr(nullptr), m_count(new size_t) { *m_count = 0; };
  shared_ptr(T* ptr) : m_ptr(ptr), m_count(new size_t) { *m_count = 1; };
  ~shared_ptr() {
    --(*m_count);
    if (*m_count == 0) {
      delete m_ptr;
      delete m_count;
      m_ptr = nullptr;
      m_count = nullptr;
    }
  }

  shared_ptr(const shared_ptr<T>& ptr)
      : m_ptr(ptr.m_ptr), m_count(ptr.m_count) {
    ++(*m_count);
  }

  shared_ptr<T>& operator=(const shared_ptr<T>& ptr) {
    if (&ptr == this) return *this;
    m_ptr = ptr.m_ptr;
    m_count = ptr.m_count;
    ++(*m_count);
    return *this;
  }

  shared_ptr(shared_ptr<T>&& ptr) : m_ptr(ptr.m_ptr), m_count(ptr.m_count) {
    ++(*m_count);
  }

  shared_ptr<T>& operator=(shared_ptr<T>&& ptr) {
    if (&ptr == this) return *this;
    m_ptr = ptr.m_ptr;
    m_count = ptr.m_count;
    ++(*m_count);
    return *this;
  }

  T& operator*() { return *m_ptr; };

  T* operator->() { return m_ptr; };

  operator bool() { return m_ptr != nullptr; };

  T* get() { return m_ptr; };

  size_t use_count() { return *m_count; };

  bool unique() { return *m_count == 1; };

  void swap(shared_ptr& ptr) { std::swap(*this, ptr); }

  void reset(T* ptr) { shared_ptr(ptr).swap(*this); }
};
}  // namespace ptr

void test_shared_ptr() {
  cout << sizeof(ptr::shared_ptr<int>) << endl;
  cout << sizeof(std::shared_ptr<int>) << endl;
  ptr::shared_ptr<int> a = new int(100);
  auto b = a;
  cout << *a << endl;
  cout << *b << endl;
}

int main(int argc, char* argv[]) {
  test_shared_ptr();
  return 0;
}