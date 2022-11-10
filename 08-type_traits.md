#### Integral_constant

```cpp
template <class T, T val>
struct integral_constant {
  static constexpr T value = val;

  using value_type = T;
  using type = integral_constant;

  constexpr operator value_type() const noexcept { return (value); }	// 类型转换
  constexpr value_type operator()() const noexcept { return (value); }	// 仿函数
};
```

#### bool_constant

```cpp
template <bool val>
using bool_constant = integral_constant<bool, val>;

using true_type = bool_constant<true>;
using false_type = bool_constant<false>;
```

#### enable_if

```cpp
template <bool test, class T = void>
struct enable_if {};

template <class T>
struct enable_if<true, T> { using type = T; };

template <bool test, class T = void>
using enable_if_t = typename enable_if<test, T>::type;
```

#### conditional

```cpp
template <bool test, class T, class U>
struct conditional {
  using type = U;
};

template <class T, class U>
struct conditional<true, T, U> {
  using type = T;
};

template <bool test, class T, class U>
using conditional_t = typename conditional<test, T, U>::type;
```

#### is_same

```cpp
template <class T, class U>
struct is_same : false_type {};

template <class T>
struct is_same<T, T> : true_type {};

template <class T, class U>
inline constexpr bool is_same_v = is_same<T, U>::value;
```

#### remove_const

```cpp
template <class T>
struct remove_const {
  using type = T;
};

template <class T>
struct remove_const<const T> {
  using type = T;
};

template <class T>
using remove_const_t = typename remove_const<T>::type;
```

#### remove_volatile

```cpp
template <class T>
struct remove_volatile {
  using type = T;
};

template <class T>
struct remove_volatile<volatile T> {
  using type = T;
};

template <class T>
using remove_volatile_t = typename remove_volatile<T>::type;
```

#### remove_cv

```cpp
template <class T>
struct remove_cv {
  using type = T;
};

template <class T>
struct remove_cv<const T> {
  using type = T;
};

template <class T>
struct remove_cv<volatile T> {
  using type = T;
};

template <class T>
struct remove_cv<const volatile T> {
  using type = T;
};

template <class T>
using remove_cv_t = typename remove_cv<T>::type;
```

#### is_integral

```cpp
template <class T>
struct _Is_integral : false_type {};

template <>
struct _Is_integral<bool> : true_type {};

template <>
struct _Is_integral<char> : true_type {};

// ...

template <>
struct _Is_integral<long long> : true_type {};

template <class T>
struct is_integral : _Is_integral<remove_cv_t<T>>::type {};

template <class T>
inline constexpr bool is_integral_v = is_integral<T>::value;
```

#### is_floating_point

```cpp
template <class T>
struct _Is_floating_point : false_type {};

template <>
struct _Is_floating_point<float> : true_type {};

template <>
struct _Is_floating_point<double> : true_type {};

template <>
struct _Is_floating_point<long double> : true_type {};

template <class T>
struct is_floating_point : _Is_floating_point<remove_cv_t<T>>::type {};

template <class T>
inline constexpr bool is_floating_point_v = is_floating_point<T>::value;
```

#### is_arithmetic

```cpp
template <class T>
struct is_arithmetic : bool_constant<is_integral_v<T> || is_floating_point_v<T>> {};

template <class T>
inline constexpr bool is_arithmetic_v = is_arithmetic<T>::value;
```

#### remove_reference

```cpp
template <class T>
struct remove_reference {
  using type = T;
};

template <class T>
struct remove_reference<T&> {
  using type = T;
};

template <class T>
struct remove_reference<T&&> {
  using type = T;
};

template <class T>
using remove_reference_t = typename remove_reference<T>::type;
```

#### conjunction：所有模板类的 value 均为 true，则 conjunction::value 为 true，否则为 false。逻辑与函数。

```cpp
template <bool _First_value, class _First,
          class... _Rest>
struct _Conjunction {  // handle false trait or last trait
  using type = _First;
};

template <class _True, class _Next, class... _Rest>
struct _Conjunction<true, _True, _Next,
                    _Rest...> {  // the first trait is true, try the next one
  using type = typename _Conjunction<_Next::value, _Next, _Rest...>::type;
};

template <class... _Traits>
struct conjunction : true_type {  // If _Traits is empty, true_type
};

template <class _First, class... _Rest>
struct conjunction<_First, _Rest...>
    : _Conjunction<_First::value, _First, _Rest...>::
          type {  // Otherwise, if any of _Traits are false, the first false
                  // trait Otherwise, the last trait in _Traits
};

template <class... _Traits>
inline constexpr bool conjunction_v = conjunction<_Traits...>::value;
```

#### disjunction：所有模板类的 value 均为 false，则 conjunction::value 为 false，否则为 true。逻辑或函数。

```cpp
template <bool _First_value, class _First,
          class... _Rest>
struct _Disjunction {  // handle true trait or last trait
  using type = _First;
};

template <class _False, class _Next, class... _Rest>
struct _Disjunction<false, _False, _Next,
                    _Rest...> {  // first trait is false, try the next trait
  using type = typename _Disjunction<_Next::value, _Next, _Rest...>::type;
};

template <class... _Traits>
struct disjunction : false_type {  // If _Traits is empty, false_type
};

template <class _First, class... _Rest>
struct disjunction<_First, _Rest...>
    : _Disjunction<_First::value, _First, _Rest...>::
          type {  // Otherwise, if any of _Traits are true, the first true trait
                  // Otherwise, the last trait in _Traits
};

template <class... _Traits>
_INLINE_VAR constexpr bool disjunction_v = disjunction<_Traits...>::value;
```

#### negation：模板类的 value 为 false，则 negation::value 为 true，否则为 false。逻辑非函数。

```cpp
template <class _Trait>
struct negation : bool_constant<!static_cast<bool>(_Trait::value)> {};

template <class _Trait>
inline constexpr bool negation_v = negation<_Trait>::value;
```

#### _Is_any_of_v：第 2~n 个模板类是否有至少一个类与第一个类是相同的类，如果是，则 _Is_any_of_v 为 true，否则为 false。

```cpp
template <class _Ty, class... _Types>
inline constexpr bool _Is_any_of_v = disjunction_v<is_same<_Ty, _Types>...>;
```

#### std::move

```cpp
template <typename T>
typename remove_reference<T>::type &&move(T &&t) {
  return static_cast<typename remove_reference<T>::type &&>(t);
}
```

#### std::forward

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

#### type_traits 实例：判断一个类的对象是否可以直接 cout

```cpp
#include <iostream>
#include <type_traits>
using namespace std;

// 类A：支持 cout 操作
struct A {
  int value;
  friend ostream &operator<<(ostream &os, const A &obj);
};
ostream &operator<<(ostream &os, const A &obj) { return os << obj.value; }

// 类B：不支持 cout 操作
struct B {};

template <class T>
struct opt_override {
  // 默认函数，输入参数为可变参数类型
  constexpr static bool hascout(...) { return false; }
  
  // 重载函数1，输入参数为 U*，且要求存在类型U的输出操作符重载
  template <class U, ostream &(*)(ostream &, const U &) = (operator<<)>
  constexpr static bool hascout(U *) {
    return true;
  }
    
  // 重载函数2，输入参数为 U*，且要求类型U必须为基本数据类型或std::string
#if defined(__GNUC__)
  template <class U, enable_if_t<is_arithmetic_v<U>> * = nullptr>
  // or: template <class U, class = enable_if_t<is_arithmetic_v<U>>>
#elif defined(_MSC_VER)
  template <class U, enable_if_t<is_arithmetic_v<U> || is_same_v<U, string>> * = nullptr>
  // or: template <class U, class = enable_if_t<is_arithmetic_v<U> || is_same_v<U, string>>>
#else
#error("undefined compiler")
#endif
  constexpr static bool hascout(U *) {
    return true;
  }
  
  constexpr static bool cout = hascout((T *)0);
};

int main(int argc, char *argv[]) {
  cout << opt_override<A>::cout << endl;             // 1
  cout << opt_override<B>::cout << endl;             // 0
  cout << opt_override<int>::cout << endl;           // 1
  cout << opt_override<double>::cout << endl;        // 1
  cout << opt_override<std::string>::cout << endl;   // 1
  return 0;
}
```

- opt_override类的成员函数存在三种形式的重载，两个模板函数是不同的重载函数，而非偏特化（因为他们的模板参数不同）
- opt_override\<T>::cout 的取值情形：
  - 当 opt_override 的模板参数为一个重载了输出操作符的类的时候，可以精确匹配到重载函数1，因而最终 opt_override\<T>::cout = true。
  - 当 opt_override 的模板参数为一个未重载输出操作符的类的时候，2个重载函数匹配失败，默认函数为不精确匹配（函数输入参数不完全匹配，但是可以自动进行类型转换），因而最终 opt_override\<T>::cout = false。
  - 当 opt_override 的模板参数为一个基本数据类型或string类的时候，可以精确匹配到重载函数2，因而最终 opt_override\<T>::cout = true。
- 重载函数的匹配规则：
  - 不允许出现多个精确匹配的模板重载函数，此次会编译报错。
  - 不允许出现多个精确匹配的非模板重载函数，此时会编译报错。
  - 可以同时出现一个精确匹配的非模板函数和一个精确匹配的模板函数，此时会优先匹配到非模板函数。
  - 模板函数要么能够精确匹配，要么匹配失败，不存在不精确匹配的情况。
  - 若精确匹配失败，则仅允许出现一个不精确匹配的非模板函数，或者出现一个不精确匹配的非模板函数和一个不定参数的非模板函数，此时会优先匹配到不精确匹配的非模板函数。
