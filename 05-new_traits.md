#### nullptr

- NULL的问题

  ```cpp
  #define NULL (void*)0
  // C语言中，考虑下面两个重载函数  
  void foo(int i);  
  void foo(char* p)  
    
  foo(NULL); // compile error
  ```

- nullptr

  ```c
  // 这里C++ 中的NULL如果和C语言一样也是(void *)0指针，而C++ 却又不允许void*隐式转换成其它指针类型，那还怎么用NULL来表示空指针呢，岂不是尴尬了。
  // 因而在C++中，NULL却被明确定义为整常数0，以保证兼容，用nullptr表示空指针
  #ifndef NULL
      #ifdef __cplusplus
          #define NULL 0
      #else
          #define NULL ((void *)0)
      #endif
  #endif
  
  // nullptr是一个c++关键字，也是一个常量，nullptr的类型由编译器指定，其类型重载了：
  // operator T*()       类外部指针强制转换
  // operator T C::*()   类内部指针强制转换
  // operator &()=delete 禁止取地址
  // operator bool()     bool转换
  // operator==()        比较
  // operator!=()        比较
  typdef decltype(nullptr) nullptr_t;
  ```

#### auto与decltype

- ***auto***：自动推导变量类型

  - 规则1：在不声明为引用或指针时，*auto* 会忽略等号右边的引用类型、*const* 属性和 *volatile* 属性
  - 规则2：在声明为引用或者指针时，*auto* 会保留等号右边的引用类型、*const* 属性和 *volatile* 属性
  - 规则3：*auto &&* 遵循万能引用规则

  ```cpp
  int i = 0;
  auto *a = &i; 	// a 是 int *
  auto &b =  i; 	// b 是 int&
  auto c  =  b;	// c 是 int  ==> 规则1
  auto &d =  b;	// d 是 int& ==> 规则2
  auto&&e =  d;	// e 是 int& ==> 规则3
  ```

- ***decltype***：自动推导表达式类型

  - *exp* 是表达式，*decltype(exp)* 和 *exp* 类型相同
  - *exp* 是函数调用，*decltype(exp)* 和函数返回值类型相同
  - 其它情况，若 *exp* 是左值，*decltype(exp)* 是 *exp* 类型的左值引用

  ```cpp
    int a = 0, b = 0;
    decltype(a + b) c = 0;   // c 是 int，因为(a+b)返回一个右值
    decltype(a += b) d = c;  // d 是 int&，因为(a+=b)返回一个左值
    decltype((a)) e = c;	   // e 是 int&，因为(a)返回一个左值
  ```

- ***auto*** 和 ***decltype*** 配合使用：返回类型后置

  ```cpp
  template <typename T, typename U>
  auto add(T t, U u) -> decltype(t + u) {
    return t + u;
  }
  ```

#### 左值引用、右值引用、移动语义、万能引用、引用折叠、完美转发

- 左值与右值

  - 可以放到等号左边的东西叫左值。 不可以放到等号左边的东西就叫右值。
  - 可以取地址并且有名字的东西就是左值。不能取地址的没有名字的东西就是右值。

- 左值举例

  - 函数名和变量名
  - 左值引用
  - 右值引用
  - 前置自增自减表达式++i、--i
  - 由赋值表达式或赋值运算符连接的表达式(a=b, a += b 等)
  - 解引用表达式 *p
  - 字符串字面值 "abcd"

- 右值举例

  - 除字符串字面值外的字面值（1、1.2、'c'、...）
  - 返回非引用类型的函数调用
  - 后置自增自减表达式 i++、i--
  - 算术表达式(a+b, a*b, a&&b, a==b 等)
  - 取地址表达式等(&a)

- 左值引用与右值引用

  - 左值引用就是对左值进行引用的类型，右值引用就是对右值进行引用的类型。
  - 他们都是引用， 都是对象的一个别名，并不拥有所绑定对象的堆存，所以都必须立即初始化。
  - 如果使用右值引用，那表达式等号右边的值需要是右值，可以使用 *std::move* 函数强制把左值转换为右值。
  - **左值引用和右值引用本身是左值**

- 移动语义

  - *std::move(a)*：需要保证变量 *a* 不再被使用
  - 移动构造函数
  - 移动赋值函数

- 万能引用

  - 模板

    - 如果我们给 *func* 传递了左值，那么 *T* 被推导为该左值的类型的左值引用，万能引用 *T &&* 就变成了左值引用
    - 如果我们给 *func* 传递了右值，那么 *T* 被推导为该右值的实际类型，万能引用 *T &&* 就变成了右值引用
    - 注意：*T &&* 才是万能引用，*T* 不是万能引用
    - 不定参数模板的万能引用：*Args &&...*

    ```cpp
    template <typename T>
    void func(T &&val);                               // T && 为万能引用
    
    template <typename Callable, typename... Args>
    void func(Callable &&f, Args &&... args);         // Callable &&、Args &&... 为万能引用
    ```

  - ***auto***

    - 如果右边变量为左值，那么 *auto* 被推导为该左值的类型的左值引用，万能引用就变成了左值引用
    - 如果右边变量为右值，那么 *auto* 被推导为该右值的实际类型，万能引用就变成了右值引用
    - 注意：*auto &&* 才是万能引用，*auto* 不是万能引用

    ```cpp
    #include <iostream>
    using namespace std;
    
    int integer = 10;
    int &int_ref = integer;
    const int const_int = integer;
    const int &const_int_ref = integer;
    int int_arr[10];
    int *int_ptr;
    int &&int_rref = 100;
    
    int main() {
      auto &&a1 = integer;        // a1: int &
      auto &&a2 = int_ref;        // a2: int &
      auto &&a3 = const_int;      // a3: int const&
      auto &&a4 = const_int_ref;  // a4: int const&
      auto &&a5 = int_arr;        // a5: int (&) [10]
      auto &&a6 = int_ptr;        // a6: int* &
      auto &&a7 = 100;            // a7: int &&
      auto &&a8 = int_rref;       // a8: int &
      return 0;
    }
    ```

- 模板类型推断

  ```cpp
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
      					  // integer 是一个左值，万能引用输入为左值时，T 被推断为左值类型的左值引用，T 被推断为 int &，
      					  // 而 val 的类型为 T &&，即 int & &&，根据引用折叠，val 的类型为 int &
  
    func_trr(int_ref);        // T: int &, val: int &
      					  // int_ref 是一个左值引用，int_ref 本身是一个左值，万能引用输入为左值时，T 被推断为左值类型的左值引用，
      					  // 而 int_ref 这个左值的类型为 int &，则 T 被推断为 int & &，再根据引用折叠，T 最终为 int &
      					  // 而 val 的类型为 T &&，即 int & &&，根据引用折叠，val 的类型为 int &
  
    func_trr(const_int);      // T: int const&, val: int const&
      					  // const_int 是一个左值，万能引用输入为左值时，T 被推断为左值类型的左值引用，T 被推断为 int const&，
      					  // 而 val 的类型为 T &&，即 int const& &&，根据引用折叠，val 的类型为 int const&
      
    func_trr(const_int_ref);  // T: int const&, val: int const&
      					  // const_int_ref 是一个左值引用，const_int_ref 本身是一个左值，万能引用输入为左值时，T 被推断为左值类型的左值引
                              // 用，而 const_int_ref 这个左值的类型为：int const&，则 T 被推断为 int const& &，再根据引用折叠，T 最终为
                              // int const&，而 val 的类型为 T &&，即 int const& &&，根据引用折叠，val 的类型为 int const&
      
    func_trr(int_arr);        // T: int (&) [10], val: int (&) [10]
      					  // int_arr 是一个左值，万能引用输入为左值时，T 被推断为左值类型的左值引用，而 int_arr 这个左值的类型为 int [10],
      					  // 则 T 被推断为 int (&) [10]，而 val 的类型为 T &&，即 int (& &&) [10]，
      					  // 再根据引用折叠，val 的类型为 int (&)[10]
      
    func_trr(int_ptr);        // T: int* &, val: int* &
      					  // int_ptr 是一个左值，，万能引用输入为左值时，T 被推断为左值类型的左值引用，而 int_ptr 这个左值的类型为 int*，
      					  // 则 T 被推断为 int* &，而 val 的类型为 T &&，即 int* & &&，根据引用折叠，val 的类型为 int *&
      
    func_trr(100);            // T: int, val: int &&
      					  // 100 是一个右值，万能引用输入为右值时，T 被推断为该右值的实际类型，而 100 这个立即数的类型为 int，则 T 被推断为
      					  // int，而 val 的类型为 T &&，即 int &&，根据引用折叠，val 的类型为 int &&
      
    func_trr(int_rref);       // T: int &, val: int &
      					  // int_rref 是一个右值引用，int_rref 本身是一个左值，万能引用输入为左值时，T 被推断为左值类型的左值引用，
      					  // 而 int_rref 这个左值的类型为 int &&，则 T 被推断为 int && &，再根据引用折叠，T 最终为 int &
      					  // 而 val 的类型为 T &&，即 int & &&，根据引用折叠，val 的类型为 int &
  }
  
  int main() {
    test_func_t();
    test_func_tr();
    test_func_ctr();
    test_func_trr();
    return 0;
  }
  ```

- 引用折叠：有左值引用，结果一定为左值引用，否则为右值引用

  - *T*  +  *&*  ==>  *T &*
  - *T*  +  *&&*  ==>  *T &&*
  - *T &*  +  *&*  ==>  *T &*
  - *T &*  +  *&&*  ==>  *T &*
  - *T &&*  +  *&*  ==>  *T &*
  - *T &&*  +  *&&*  ==>  *T &&*

- 完美转发

  - 完美转发指可以写一个接受任意实参的函数模板，并转发到其它函数，目标函数会收到与转发函数完全相同的实参，转发函数实参是左值那目标函数实参也是左值，转发函数实参是右值那目标函数实参也是右值。

  - 实现方式：*std::forward*

  - *std::forward\<T>(a)* 的 最终类型为 *T &&*，并遵循引用折叠

    ```cpp
    void PrintV(int &t) { cout << "lvalue" << endl; }
    void PrintV(int &&t) { cout << "rvalue" << endl; }
    template <typename T>
    void Test(T &&t) {
      PrintV(t);
      PrintV(std::forward<T>(t));
      PrintV(std::move(t));
    }
    
    int a = 1;
    int &&b = 1;
    
    int main() {
      Test(1);                        // lvalue rvalue rvalue
      Test(a);                        // lvalue lvalue rvalue
      Test(b);                        // lvalue lvalue rvalue
      Test(std::forward<int>(1));     // lvalue rvalue rvalue
      Test(std::forward<int>(a));     // lvalue rvalue rvalue
      Test(std::forward<int &>(a));   // lvalue lvalue rvalue
      Test(std::forward<int &&>(a));  // lvalue rvalue rvalue
      return 0;
    }
    ```

- *std::move* 与 *std::forward* 的实现原理

  - *std::remove_reference\<T>*：用于移除 *T* 的左引用/右引用属性

  ```cpp
  // 泛化类
  template <typename T>
  struct remove_reference {
    using type = T;
  };
  
  // 偏特化：左引用类型
  template <typename T>
  struct remove_reference<_Ty&> {
    using type = T;
  };
  
  // 偏特化：右引用类型
  template <typename T>
  struct remove_reference<_Ty&&> {
    using type = T;
  };
  
  int main() {
    using type_a = remove_reference<int>::type;     // 匹配到泛化版本，type_a = int
    using type_b = remove_reference<int &>::type;   // 匹配到左引用特化版本，type_b = int
    using type_c = remove_reference<int &&>::type;  // 匹配到右引用特化版本，type_c = int
      										  // 如果没有这两个特化版本，则 type_a = int, type_b = int &, type_c = int &&, 
      										  // 无法获取 T 类型去除引用后的类型
    return 0;
  }
  ```

  - *std::move* 实现原理：通过 *remove_reference*，得到去除引用的类型，最后将变量强制转换为去除引用后的类型的右引用并返回

  ```cpp
  template <typename T>
  typename remove_reference<T>::type &&move(T &&t) {
    return static_cast<typename remove_reference<T>::type &&>(t);
  }
  ```

  - *std::forward* 实现原理：通过 *remove_reference*，得到去除引用的类型，分别实现接收左值和右值的重载函数，强制转换 *static_cast<T &&>* 时， *T &&* 遵循引用折叠规则

  ```cpp
  template <typename T>
  T &&forward(typename std::remove_reference<T>::type &t) {
    return static_cast<T &&>(t);
  }
  
  template <typename T>
  T &&forward(typename std::remove_reference<T>::type &&t) {
    return static_cast<T &&>(t);
  }
  ```

#### std::function

- 可调用对象：
  - 函数指针
  - 是一个具有 *operator()* 成员函数的类对象（仿函数）
  - *lambda* 表达式
  - 是一个可被转换为函数指针的类对象
  - 是一个类成员(函数)指针
  - *bind* 表达式或其它函数对象
- *std::function*：可调用对象的封装器，头文件：*\<functional>*
- *std::bind*：可以将可调用对象和参数一起绑定，绑定后的结果使用 *std::function* 进行保存，并延迟调用到任何我们需要的时候。
  - 可以将可调用对象与参数一起绑定为另一个 *std::function* 供调用
  - 将 *n* 元可调用对象转成 *m(m < n)* 元可调用对象，绑定一部分参数，这里需要使用 *std::placeholders*

#### lambda 表达式

- 语法形式：*[ capture-list ] ( params ) mutable(optional) exception(optional) attribute(optional) -> ret(optional) { body }*

  - *capture-list*：捕获列表。
    - []不捕获任何变量
    - [&]引用捕获，捕获外部作用域所有变量，在函数体内当作引用使用
    - [=]值捕获，捕获外部作用域所有变量，在函数内内有个副本使用
    - [=, &a]值捕获外部作用域所有变量，按引用捕获 a 变量
    - [a]只值捕获 a 变量，不捕获其它变量
    - [this]捕获当前类中的 this 指针
  - *params*：和普通函数一样的参数。
  - *mutable*：只有这个 *Lambda* 表达式是 *mutable* 的才允许修改按值捕获的参数。
  - *exception*：异常标识。
  - *attribute*：属性标识。
  - *ret*：返回值类型，可以省略，让编译器通过 return 语句自动推导。
  - *body*：函数的具体逻辑。

- *lambda* 表达式就相当于是一个仿函数，仿函数是一个 有 *operator()* 成员函数的类对象，这个 *operator()* 默认是 *const* 的，所以不能修改成员变量，而加了 *mutable*，就是去掉 *const* 属性。

- 如果可以推导出返回值类型，则可以省略 箭头和 *ret*

  ```cpp
  auto f = [](int i){ return 2 * i; };
  ```

- 没有参数列表时，参数列表是可以省略的

  ```cpp
  auto f = [] { return 1; };
  ```

#### 委托构造函数

- 委托构造函数允许在同一个类中一个构造函数调用另外一个构造函数，可以在变量初始化时简化操作

```cpp
struct A {
  A() {}
  A(int a) { a_ = a; }
  A(int a, int b) : A(a) { b_ = b; }
  A(int a, int b, int c) : A(a, b) { c_ = c; }
  int a_;
  int b_;
  int c_;
};
```

#### 继承构造函数

```cpp
struct Base {
  Base() {}
  Base(int a) { a_ = a; }
  Base(int a, int b) : Base(a) { b_ = b; }
  Base(int a, int b, int c) : Base(a, b) { c_ = c; }
  int a_;
  int b_;
  int c_;
};
struct Derived : Base {
  using Base::Base;
  // equals with following
  /**
   * Derived() {}
   * Derived(int a) : Base(a) {}                      // 好麻烦
   * Derived(int a, int b) : Base(a, b) {}            // 好麻烦
   * Derived(int a, int b, int c) : Base(a, b, c) {}  // 好麻烦
   */
};
int main() {
  Derived a(1, 2, 3);
  return 0;
}
```

#### nullptr

- ***NULL*** 本质上是个 *int* 型的 0，其实不是个指针
- *nullptr* 是 c++11 用来表示空指针新引入的常量

#### final & override

- *final* 用于修饰一个类，表示禁止该类进一步派生和虚函数的进一步重载
- *override* 用于修 饰派生类中的成员函数，标明该函数重写了基类函数

#### default

- 用于声明构造函数为默认构造函数

#### delete

- 禁用默认生成的类成员函数（拷贝构造，赋值等）

#### explicit

- 专用于修饰构造函数，表示只能显式构造，不可以被隐式转换

#### const 与 constexpr

- 两者都代表可读，*const* 只表示 *read only* 的语义，只保证了运行时不可以被修改，但它修饰的仍然有可能是个动态变量
- *constexpr* 修饰的才是真正的常量，它会在编译期间就会被计算出来，整个运行过程中都不可以被改变
- *constexpr* 可以用于修 饰函数，这个函数的返回值会尽可能在编译期间被计算出来当作一个常量，但是如果编译期间此函数不能被计算出来，那它就 会当作一个普通函数被处理。

#### enum class

- *enum class* 枚举必须带有类作用域：

  ```cpp
  enum class A { A_a, A_b };
  A_a;	// error
  A::A_a;	// passed
  ```

- *enum class* 枚举禁止不同的枚举类之间的比较

#### static_assert

```cpp
static_assert(true/false, message);
```

- 用于在编译期间检查，如果第一个参数值为 *false*，则打印 *message*，编译失败。

#### 正则表达式

- 头文件 \<regex>

#### chrono

- duration：表示一段时间
- time_point：表示时间点
- clocks：表示时钟类型
  - steady_clock：稳定时钟
  - system_clock：系统时钟
  - high_resolution_clock：高精度时钟，稳定时钟或系统时钟的其中一种，不同系统定义不同

#### 智能指针

- ***unique_ptr***：独占资源所有权的指针

  ```cpp
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
    tuple<T*, U> m_tu;	// 使用tuple，删除器不占用额外内存
  };
  ```
  
- ***shared_ptr***：共享资源所有权的指针

  - 通过 *shared_from_this()* 返回 *this* 指针，不要把 *this* 指针作为 *shared_ptr* 返回出来，因为 *this* 指针本质就 是裸指针，通过 *this* 返回可能会导致重复析构，不能把 this 指针交给智能指针管理。
  - *shared_from_this()* 其实就是通过 *weak_ptr* 返回的 *this* 指针。

  ```cpp
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
          if(*m_count == 0) {
              delete m_ptr;
              delete m_count;
              m_ptr = nullptr;
              m_count = nullptr;
          }
      }
  
      shared_ptr(const shared_ptr<T>& ptr) : m_ptr(ptr.m_ptr), m_count(ptr.m_count) {
          ++(*m_count);
      }
  
      shared_ptr<T> &operator=(const shared_ptr<T>& ptr) {
          if (&ptr == this) return *this;
          m_ptr = ptr.m_ptr;
          m_count = ptr.m_count;
          ++(*m_count);
          return *this;
      }
  
      shared_ptr(shared_ptr<T>&& ptr) : m_ptr(ptr.m_ptr), m_count(ptr.m_count) {
          ++(*m_count);
      }
  
      shared_ptr<T> &operator=(shared_ptr<T>&& ptr) {
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
  
      void swap(shared_ptr& ptr) {
          std::swap(*this, ptr);
      }
      
      void reset(T *ptr) {
          shared_ptr(ptr).swap(*this);
      }
  };
  ```

- ***weak_ptr***：共享资源的观察者

  | 成员函数          | 功能                                                         |
  | ----------------- | ------------------------------------------------------------ |
  | ***operator=()*** | 重载 = 赋值运算符，***weak_ptr*** 指针可以直接被 ***weak_ptr*** 或 ***shared_ptr*** 类型指针赋值。 |
  | ***swap(x)***     | 该函数可以互换 2 个同类型 ***weak_ptr*** 指针的内容。        |
  | ***reset()***     | 将当前 ***weak_ptr*** 指针置为空指针。                       |
  | ***use_count()*** | 查看指向和当前 ***weak_ptr*** 指针相同的 ***shared_ptr*** 指针的数量。 |
  | ***expired()***   | 判断当前 ***weak_ptr*** 指针为否过期。                       |
  | ***lock()***      | 如果当前 ***weak_ptr*** 已经过期，则该函数会返回一个空的 ***shared_ptr*** 指针；反之，该函数返回一个和当前 ***weak_ptr*** 指向相同的 ***shared_ptr*** 指针。 |

- 循环引用

  ```cpp
  struct A;
  struct B;
  struct A {
    std::shared_ptr<B> bptr;
    ~A() { cout << "A delete" << endl; }
  };
  struct B {
    std::shared_ptr<A> aptr;
    ~B() { cout << "B delete" << endl; }
  };
  int main() {
    auto aaptr = std::make_shared<A>();
    auto bbptr = std::make_shared<B>();
    aaptr->bptr = bbptr;
    bbptr->aptr = aaptr;
    return 0;
  }
  ```

- 循环引用解决方法：使用 *weakptr*

  ```cpp
  struct A;
  struct B;
  struct A {
    std::shared_ptr<B> bptr;
    ~A() { cout << "A delete" << endl; }
    void Print() { cout << "A" << endl; }
  };
  struct B {
    std::weak_ptr<A> aptr;  // 这里改成 weak_ptr
    ~B() { cout << "B delete" << endl; }
    void PrintA() {
      if (!aptr.expired()) {  // 监视 shared_ptr 的生命周期
        auto ptr = aptr.lock();
        ptr->Print();
      }
    }
  };
  int main() {
    auto aaptr = std::make_shared<A>();
    auto bbptr = std::make_shared<B>();
    aaptr->bptr = bbptr;
    bbptr->aptr = aaptr;
    bbptr->PrintA();
    return 0;
  }
  ```

  