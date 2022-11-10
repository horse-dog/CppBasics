#### 默认模板参数

- 对于类模板，默认模板参数必须从右到左

- 对于函数模板，没有限制

  ```cpp
  template <typename T, typename U=int>
  class A {
  	T value;
  };
  
  template <typename T=int, typename U> // error
  class A {
  	T value;
  };
  ```

#### 模板泛化，全特化，偏特化

- 类模板泛化

  ```cpp
  template <typename T, typename U>
  class A {
      A() { cout << "泛化" << endl; }
      ~A() {};
  };
  ```

- 类模板全特化

  ```cpp
  template <>
  class A<int, double> {
      A() { cout << "int, double 全特化" << endl; }
      ~A() {};
  };
  ```

- 类模板偏特化

  - 参数数量偏特化

    ```cpp
    template <typename T>
    class A<T, double> {
        A() { cout << "T, double 偏特化" << endl; }
        ~A() {};
    };
    ```

  - 参数类型偏特化

    ```cpp
    template <typename T>
    class A {
        A() { cout << "泛化" << endl; }
        ~A() {};
    };
    
    template <typename T>
    class A<const T> {
        A() { cout << "const 特化" << endl; }
        ~A() {};
    };
    
    template <typename T>
    class A<T *> {
        A() { cout << "指针特化" << endl; }
        ~A() {};
    };
    
    template <typename T>
    class A<T &> {
        A() { cout << "左引用特化" << endl; }
        ~A() {};
    };
    
    template <typename T>
    class A<T &&> {
        A() { cout << "右引用特化" << endl; }
        ~A() {};
    };
    
    int main() {
        A<int>			a;	// 泛化
        A<const int>	b;	// const 特化
        A<int *>		c;	// 指针特化
        A<const int *>	d;	// 指针特化
        A<int * const>	e;	// const 特化
        A<int &>		f;	// 左引用特化
        A<int &&>		g;	// 右引用特化
    }
    ```

- 函数模板泛化

  ```cpp
  template <typename T, typename U>
  void func(T a, U b) {
    cout << "泛化" << endl;
  }
  ```

- 函数模板全特化

  ```cpp
  template <>
  void func<>(int a, double b) {
    cout << "int, double 全特化" << endl;
  }
  ```

- 函数模板偏特化

  - 参数数量偏特化：**无法偏特化**

    ```cpp
    template <typename T>
    void func<T, double>(T a, double b) {	// compile error
      cout << "T, double 偏特化" << endl;
    }
    
    template <typename T>
    void func(T a, double b) {				// 这是另一个函数模板，并非泛化版本的特化版本
      cout << "int, double 全特化" << endl;
    }
    ```

  - 参数类型偏特化：**无法偏特化**

#### 可变参模板

- 可变参函数模板

  ```cpp
  #include <iostream>
  using namespace std;
  
  void func() { cout << "receive end..." << endl; }
  
  template <typename T, typename... U>
  void func(const T &firstArg, const U &...otherArgs) {
    cout << "received: " << firstArg << endl;
    func(otherArgs...);
  }
  
  /**
   * 输出：
   * received: 1
   * received: 1.1
   * received: hello
   * receive end...
   */
  int main() {
    func(1, 1.1, "hello");
    return 0;
  }
  ```

- 可变参类模板：使用递归继承

  ```cpp
  #include <iostream>
  using namespace std;
  
  template <typename... Args>
  class A {};
  
  template <>
  class A<> {
   public:
    A() { cout << "A<>::A() called..." << endl; }
  };
  
  template <typename _First, typename... _Others>
  class A<_First, _Others...> : private A<_Others...> {
   public:
    A(_First first, _Others... others) : m_i(first), A<_Others...>(others...) {
      cout << "A::A(first, others) called..., m_i = " << m_i << endl;
    }
    _First m_i;
  };
  
  /**
   * 输出：
   * A<>::A() called...
   * A::A(first, others) called..., m_i = 3.14
   * A::A(first, others) called..., m_i = 2.1
   * A::A(first, others) called..., m_i = 1
   */
  int main() {
    A<int, float, double> myc(1, 2.1f, 3.14);
    return 0;
  }
  ```

- 可变参类模板：使用递归组合

  ```cpp
  #include <iostream>
  using namespace std;
  
  template <typename... Args>
  class A {};
  
  template <>
  class A<> {
   public:
    A() { cout << "A<>::A() called..." << endl; }
  };
  
  template <typename _First, typename... _Others>
  class A<_First, _Others...> {
   public:
    A(_First first, _Others... others) : m_i(first), m_o(others...) {
      cout << "A::A(first, others) called..., m_i = " << m_i << endl;
    }
    _First m_i;
    A<_Others...> m_o;
  };
  
  /**
   * 输出：
   * A<>::A() called...
   * A::A(first, others) called..., m_i = 3.14
   * A::A(first, others) called..., m_i = 2.1
   * A::A(first, others) called..., m_i = 1
   */
  int main() {
    A<int, float, double> myc(1, 2.1f, 3.14);
    return 0;
  }
  ```
  
- 可变模板参数的万能引用

  ```cpp
  template <typename Callable, typename... Args>
  void func(Callable &&f, Args &&... args);         // Callable &&、Args &&... 为万能引用
  ```

- 在非可变模板参数的类 / 函数中使用多参数

  ```cpp
  #include <iostream>
  using namespace std;
  
  template <class Callable, class... Args>
  void func(Callable &&fn, Args &&...args) {
    cout << disjunction_v<is_arithmetic<Args>...> << endl;
    fn(forward<Args>(args)...);
  }
  
  int main(int argc, char *argv[]) {
    func(printf, "there're %d, %lf, %s\n", 1, 1.2, "cplusplus");
    return 0;
  }
  ```

  - is_arithetic\<T> 是一个类模板，接收固定参数，Args... 为可变模板参数，disjunction_v\<Args...> 是一个模板，接收可变参数。
  - forward\<T>(t) 是一个函数模板，接收固定参数，Args... 为可变模板参数，args... 为可变参数，printf 是一个接收可变参数的函数。


#### 模板模板参数

```cpp
#include <iostream>
#include <vector>
using namespace std;

template <typename T>
using myvector = vector<T, allocator<T>>;

template <typename T, template <typename> typename Container>
class A {
  Container<T> c;

 public:
  void push_back(const T &val) { c.push_back(val); }
  void print() const {
    for (auto &item : c) {
      cout << item << ' ';
    }
    cout << endl;
  }
};

int main() {
  A<int, myvector> ivc;
  ivc.push_back(3);
  ivc.push_back(4);
  ivc.push_back(5);
  ivc.print();
  return 0;
}
```

