#### C语言字符串操作

- ***strcpy(dst, src)*** 复制字符串
- ***strncpy(dst, src, n)*** 复制指定长度字符串

- ***strcat(dst, src)*** 附加字符串

- ***strncat(dst, src, n)*** 附加指定长度字符串

- ***strlen(str)*** 取字符串长度

- ***strcmp(str1, str2)*** 比较字符串

- ***strcasecmp(str1, str2)***忽略大小写比较字符串

- ***strncmp(str1, str2, n)*** 比较指定长度字符串

- ***strchr(str, c)*** 在字符串中查找指定字符

- ***strrchr(str, c)*** 在字符串中反向查找

- ***strstr(str1, str2)*** 查找字符串

- ***...***

#### string类

- ***find(target, off)***：寻找 *char/char \*/string/string_view* 在字符串中第一次出现的位置，*off* 为开始寻找的下标，*off* 默认为0
- ***rfind(traget, off)***：寻找 *char/char \*/string/string_view* 在字符串中第一次出现的位置，寻找到下标 *off* 停止，*off* 默认为 *length - 1*
- ***find_first_of(target, off)***：同 *find*
- ***find_first_not_of(target, off)***：类似于 *find*，寻找第一个不满足 *target* 的下标
- ***find_last_of(target, off)***：类似于 *find*，寻找最后一个满足 *target* 的下标
- ***find_last_not_of(target, off)***：类似于 *find*，寻找最后一个不满足 *target* 的下标
- ***...***

#### string_view

- ***C++17*** 中我们可以使用 *std::string_view* 来获取一个字符串的视图

- 字符串视图并不真正的创建或者拷贝字符串，而只是拥有一个字符串的查看功能。

- *std::string_view* 比 *std::string* 的性能要高很多，因为每个 *std::string* 都独自拥有一份字符串的拷贝，而*std::string_view* 只是记录了自己对应的字符串的指针和偏移位置。

- 当我们在只是查看字符串的函数中可以直接使用 *std::string_view* 来代替 *std::string*。

  ```cpp
  #include <chrono>
  #include <iostream>
  #include <string>
  #include <string_view>
  
  // 计时器
  class Timer {
   private:
    std::string title;
    std::chrono::high_resolution_clock::time_point m_start, m_stop;
  
   public:
    Timer(const std::string& title) : title(title) {
      m_start = std::chrono::high_resolution_clock::now();
    }
    ~Timer() { stop(); }
  
    void stop() {
      m_stop = std::chrono::high_resolution_clock::now();
      std::chrono::milliseconds ms =
          std::chrono::duration_cast<std::chrono::milliseconds>(m_stop - m_start);
      std::cout << title << " " << (ms.count()) * 0.001 << "s\n";
    }
  };
  
  void FunctionWithString(const std::string& string) {}
  void FunctionWithString(const std::string_view& stringView) {}
  
  // 输出：
  // std::string 0.223s
  // std::string_view 0.008s
  int main() {
    {
      Timer timer("std::string");
      for (int i = 0; i < 1000000; i++) {
        std::string name = "Horse Dog";
        std::string firstName = name.substr(0, 5);
        std::string lastName = name.substr(6, -1);
        FunctionWithString(firstName);
        FunctionWithString(lastName);
      }
    }
  
    {
      Timer timer("std::string_view");
      for (int i = 0; i < 1000000; i++) {
        const char* name = "Horse Dog";
        std::string_view firstName = std::string_view(name, 5);
        std::string_view lastName = std::string_view(name + 6, 3);
        FunctionWithString(firstName);
        FunctionWithString(lastName);
      }
    }
    return 0;
  }
  ```


#### sstream

- ***stringstream***

  ```cpp
  template <class out_type, class in_value>
  out_type convert(const in_value& t) {
    stringstream stream;
    stream << t;  //向流中传值
    out_type result;  //这里存储转换结果
    stream >> result;  //向result中写入值
    return result;
  }
  
  int main() {
    double d;
    string salary;
    string s = "12.56";
    d = convert<double>(s);            // d等于12.56
    salary = convert<string>(9000.0);  // salary等于”9000”
  }
  ```

- ***ostringstream***

  ```cpp
  #include <iostream>
  #include <sstream>
  using namespace std;
  
  template <class T>
  void to_string(string& result, const T& t) {
    ostringstream oss;   //创建一个流
    oss << t;            //把值传递如流中
    result = oss.str();  //获取转换后的字符转并将其写入result
  }
  
  int main() {
    string str;
    to_string(str, 10.5);  // double到string
    cout << str << endl;
  }
  ```

- ***istringstream***

  ```cpp
  int main() {
    int a;
    double b;
    istringstream ism("1 3.14");
    ism >> a >> b;  // a = 1; b = 3.14
  }
  ```

  
