#### 多线程相关头文件

- *\<atomic>*：*std::atomic* 和 *std::atomic_flag*
- *\<thread>*：*std::thread* 和 *std::this_thread*
- *\<mutex>*：*std::mutex* 系列类，*std::lock_guard*，*std::unique_lock*
- *<condition_variable>*：*std::condition_variable* 和 *std::condition_variable_any*
- *\<future>*：*std::promise*, *std::package_task* 两个 *Provider* 类，以及 *std::future* 和 *std::shared_future* 两个 *Future 类*，*std::async()* 函数

#### **volatile** 关键字

*volatile* 提醒编译器它后面所定义的变量随时都有可能改变，因此编译后的程序每次需要存储或读取这个变量的时候，告诉编译器对该变量不做优化，都会直接从变量内存地址中读取数据，从而可以提供对特殊地址的稳定访问。

#### **std::this_thread**

```cpp
std::thread::id std::this_thread::get_id();               // 获取当前线程的 id
void std::this_thread::sleep_for(const duration &time);   // 阻塞当前线程一段时间
void std::this_thread::sleep_for(const time_point &time); // 阻塞当前线程到某个时间点
void std::this_thread::yield();                           // 放弃当前线程的时间片，使CPU重新调度以便其它线程执行
```

#### **thread_local**

- 用 *thread_local* 修饰的变量具有 *thread* 周期，每一个线程都拥有并只拥有一个该变量的独立实例， 一般用于需要保证线程安全的函数中。

  ```cpp
  struct A {
   public:
    A &test(const std::string &name) {
      thread_local int count = 0;
      ++count;
      cout << name << ": " << count << std::endl;
      return *this;
    }
  };
  
  void func(const string &name) {
    A a1, a2;
    a1.test(name).test(name);
    a2.test(name).test(name);
  }
  
  // 输出：
  // t1: 1
  // t1: 2
  // t1: 3
  // t1: 4
  // t2: 1
  // t2: 2
  // t2: 3
  // t2: 4
  int main(int argc, char *argv[]) {
    thread t1(func, "t1");
    t1.join();
    thread t2(func, "t2");
    t2.join();
    return 0;
  }
  ```


#### **std::thread**

- 构造函数

  ```cpp
  explicit thread(Fn&& fn, Args&&... args);
  ```

- 成员函数

  ```cpp
  void std::this_thread::sleep_for(const chrono::duration& time);      // 线程休眠
  void std::this_thread::sleep_until(const chrono::time_point& time);  // 线程休眠
  std::thread::id std::thread::get_id();                               // 获取线程id
  bool std::thread::joinable();                                        // 检查线程可否被 join
  void std::thread::join();                                            // 保证线程函数的生命周期和线程对象的生命周期相同
  void std::thread::detach();                                          // 将线程和线程对象分离 
  void std::thread::swap(std::thread &other);						   // 交换 
  std::thread::native_handle_type std::thread::native_handle();		// 获取 handle，可用于 pthread 相关操作
  static unsigned int std::thread::hardware_concurrency();		    // 获取cpu数量
  ```

- 拷贝赋值操作被禁用，thread 对象不可被拷贝。

#### **std::mutex**

- *std::mutex*：独占的互斥量，不能递归使用，不带超时功能

  ```cpp
  void std::_Mutex_base::lock();      // 上锁互斥量：(1)正常上锁; (2)互斥量被其他线程锁住,阻塞本线程; (3)互斥量被本线程锁住，产生死锁(不可重入性)
  bool std::_Mutex_base::try_lock();	// 尝试上锁：(1)正常上锁，返回true;
                                      //          (2)互斥量被其他线程锁住，返回false，且不阻塞;
                                      //          (3)互斥量被本线程锁住，产生死锁(不可重入性);
  void std::_Mutex_base::unlock();    // 解锁互斥量：(1)正常解锁; (2)对未被上锁的互斥量调用unlock，什么也不做;
  ```

- *std::recursive_mutex*：递归互斥量，可重入，不带超时功能

- *std::time_mutex*：带超时的互斥量，不能递归

  ```cpp
  // lock、try_lock、unlock 与 mutex 相同
  bool std::timed_mutex::try_lock_for(const chrono::milliseconds &time);// 一段时间内线程没有获得锁则被阻塞住，如果在此期间其他线程释放了锁，则                                                                       // 该线程可以获得对互斥量的锁，返回true，如果超时(即在指定时间内还是没                                                                       // 有获得锁)，则返回 false
  bool std::timed_mutex::try_lock_until(const chrono::time_point &time);// 与try_lock_for类似
  ```

- *std::recursive_timed_mutex*：带超时的互斥量，可以递归使用

- *std::shared_mutex*：读写互斥量，根据读写锁的定义，读写锁均不可递归使用，因而不存在 *recursive_shared_mutex* 类

  ```cpp
  // 写锁
  void std::shared_mutex::lock();
  bool std::shared_mutex::try_lock();
  void std::shared_mutex::unlock();
  
  // 读锁
  void std::shared_mutex::lock_shared();
  bool std::shared_mutex::try_lock_shared();
  void std::shared_mutex::unlock_shared();
  ```

- std::shared_timed_mutex：带超时的读写互斥量，根据读写锁的定义，读写锁均不可递归使用，因而不存在 *recursive_shared_timed_mutex* 类

#### **BasicLockable，Lockable，TimedLockable**

- *BasicLockable* 类型：满足两种操作的类：*lock* 和 *unlock* 的类型
- *Lockable* 类型：满足三种操作的类：*lock*、*unlock* 和 *try_lock* 的类型
- *TimedLockable* 类型：满足五种操作的类：*lock*、*unlock*、*try_lock*、*try_lock_for*、*try_lock_until*
- *SharedLockable* 类型：满足六种操作的类：*lock*、*unlock*、*try_lock*、*lock_shared*、*unlock_shared*、*try_lock_shared*
- *SharedTimedLockable* 类型：满足十种操作的类：*TimedLockable* 的五个函数及其 *shared* 版本

#### ***std::lock***

- 相关的 *Tag* 类

  ```cpp
  struct adopt_lock_t {};
  struct defer_lock_t {};
  struct try_to_lock_t {};
  
  constexpr adopt_lock_t adopt_lock {};
  constexpr defer_lock_t defer_lock {};
  constexpr try_to_lock_t try_to_lock {};
  ```

- *std::lock_guard*

  - 比 *unique_lock* 轻量级

  - 是一个模板类，模板参数 *Mutex* 代表互斥量类型，它应该是一个基本的 *BasicLockable* 类型

  - *locking* 初始化：在构造时对互斥量进行上锁

    ```cpp
    explicit lock_guard (mutex_type& m);
    ```

  - *adopting* 初始化：在构造时不对互斥量进行上锁

    ```cpp
    lock_guard (mutex_type& m, adopt_lock_t tag);
    ```

  - 拷贝构造和移动构造均被禁用

- *std::unique_lock*

  - 比 *lock_guard* 更灵活
  - 是一个模板类，模板参数 *Mutex* 代表互斥量类型，它应该是一个基本的 *BasicLockable* 类型
  - 初始化方式
    - *locking* 初始化：同 *lock_guard*
    - *adopting* 初始化：同 *lock_guard*
    - *try-locking* 初始化：*unique_lock(mutex_type& m, try_to_lock_t tag);*，构造函数调用互斥量的 *try_lock* 函数
    - *deferred* 初始化：*unique_lock(mutex_type& m, defer_lock_t tag) noexcept;*，在初始化的时候并不锁住互斥量，暗示之后会在 *unique_lock* 对象外部手动锁住互斥量
    - *locking_for* 初始化：*unique_lock(mutex_type& m, const duration& time);*，构造函数调用互斥量的 *lock_for* 函数，若互斥量无该函数，编译报错
    - *locking_until* 初始化：*unique_lock(mutex_type& m, const time_point& time);*，构造函数调用互斥量的 *lock_until* 函数，若互斥量无该函数，编译报错
    - 拷贝构造被禁用，但移动构造未被禁用
  - 成员函数
    - *lock*、*try_lock*、*try_lock_for*、*try_lock_until* 和 *unlock*：调用互斥量的对应函数
    - *swap*：与另一个 *unique_lock* 对象进行交换
    - *release*：返回指向它所管理的 *Mutex* 对象的指针，并释放所有权
    - *mutex*：返回所管理的 *Mutex* 对象的指针
    - *owns_lock*：返回当前 *std::unique_lock* 对象是否获得了锁
    - *operator bool()*：与 *owns_lock* 功能相同，返回当前 *std::unique_lock* 对象是否获得了锁

- *std::shared_lock*：类似于 *unique_lock*，模板参数需要是 *SharedLockable* 的，底层操作的是互斥量的 *shared* 版本函数

- *std::scoped_lock*：类似于 *lock_guard*，但可以对多个互斥量进行上锁

#### **std::lock** 函数与 **std::try_lock** 函数

- *std::lock* 函数
  - 它可以一次性 *lock* 两个或者两个以上的互斥量。
  - 它不存在因为多个线程中因为锁的顺序问题导致死锁的风险的问题。
- *std::try_lock* 函数
  - 它可以一次性 *try_lock* 两个或者两个以上的互斥量。
  - 如果某一个互斥量 *lock* 失败，函数退出并解锁已经上锁的所有互斥量。
  - 返回一个整数，在成功时返回-1，失败时会返回失败的互斥量的索引(从0开始)

#### **std::once_flag** 与 **std::call_once**

- *std::once_flag* 为一个类型，它只用默认构造函数构造，不能拷贝不能移动，表示函数的一种内在状态，使用时需要配合 *call_once* 函数。

- 多个线程同时调用某个函数，*std::call_once* 函数可以保证多个线程对该函数只调用一次。

  ```cpp
  template< class Callable, class... Args >
  void call_once( std::once_flag& flag, Callable&& f, Args&&... args );
  ```

  - flag：once_flag 类型的对象
  - f：可调用对象(函数、仿函数等)
  - args：可调用对象的输入参数

#### **std::future**、**std::promise**、**std::packaged_task**、**std::async**

- *std::promise*

  - *std::promise* 对象可以保存某一类型 *T* 的值，该值可被 *future* 对象读取（可能在另外一个线程中）

  - 可以通过 *std::promise::get_future* 来获取与该 *promise* 对象相关联的 *future* 对象

  - *std::promise* 禁用拷贝语义，保留移动语义

  - 成员函数

    ```cpp
    std::future<T> get_future();                               // 返回关联的future对象
    void set_value(val);                                       // 设置共享状态的值，此后 promise 的共享状态标志变为 ready
    void set_value_at_thread_exit(val);                        // 设置共享状态的值，但是不将共享状态的标志设置为 ready，当线程退出时该                                                                  // promise 对象会自动设置为 ready
    void set_exception(std::exception_ptr _Exc);               // 为 promise 设置异常，此后 promise 的共享状态变标志变为 ready
    void set_exception_at_thread_exit(std::exception_ptr _Exc);// 为 promise 设置异常，，但是不将共享状态的标志设置为 ready，当线程退出时该                                                              // promise 对象会自动设置为 ready
    void swap(std::promise<T> &_Other);                        // 交换函数
    ```

  - 使用示例

    ```cpp
    void func(std::future<int>& fut) {
      int x = fut.get();
      cout << "value: " << x << endl;
    }
    
    // 执行结果：5秒后输出 "value: 6"
    int main() {
      std::promise<int> prom;
      std::future<int> fut = prom.get_future();
      std::thread t(func, std::ref(fut));
      this_thread::sleep_for(5s);
      prom.set_value(6);
      t.join();
      return 0;
    }
    ```

- *std::packaged_task*

  - *std::packaged_task* 包装一个可调用的对象，将其包装的可调用对象的执行结果传递给一个 std::future 对象，允许异步获取可调用对象产生的结果

  - 可以通过 *std::packged_task::get_future* 来获取与共享状态相关联的 *std::future* 对象

  - *std::packaged_task* 禁用拷贝语义，保留移动语义

  - 成员函数

    ```cpp
    bool valid();                                       // 返回当前 packaged_task 是否包装了一个可调用对象，例如 packaged_task 默认构造后该                                                     // 函数返回 false
    std::future<T> get_future();                        // 返回关联的future对象
    void operator()(_ArgTypes... _Args);                // 操作符重载，使得 packaged_task 对象可以像可调用对象一样直接调用，并在调用后设置共享                                                     // 状态标志为ready
    void make_ready_at_thread_exit(_ArgTypes... _Args); // 调用 packaged_task 的可调用对象，但是不将共享状态的标志设置为 ready，当线程退出时                                                     // 该 packaged_task 对象会自动设置为 ready
    void reset();                                       // 重置 packaged_task 的共享状态，但是保留之前的被包装的任务
    void swap(std::packaged_task<T> &_Other);           // 交换函数
    ```

  - 使用示例

    ```cpp
    int func(int in) {
      this_thread::sleep_for(5s);
      return in + 1;
    }
    
    // 执行结果：5秒后输出 "value: 6", 再过5秒后输出 "value: 11"
    int main() {
      std::packaged_task<int(int)> task(func);
      
      auto fut = task.get_future();
      std::thread(std::ref(task), 5).join();
      cout << "result: " << fut.get() << endl;
      
      task.reset();
      fut = task.get_future();
      std::thread(std::ref(task), 10).join();
      cout << "result: " << fut.get() << endl;
      return 0;
    }
    ```

- *std::async*

  - *std::async* 用于创建异步任务，实际上就是创建一个线程异步执行相应任务，它接受回调（即函数或函数对象）作为参数。

  - *std::async* 是异步编程的高级封装，相当于封装了 *std::promise*、*std::packaged_task* 和 *std::thread*，基本上可以代替 *std::thread* 的所有事情。

    ```cpp
    future async(Fn&& fn, Args&&... args);				// 采用系统默认启动策略
    future async(launch policy, Fn&& fn, Args&&... args);// 采用指定启动策略
    ```

    - *policy*：启动策略，以下3种取值之一
      - *launch::async*：它保证了异步行为，即传递的函数将在单独的线程中执行。
      - *launch::deferred*：非异步行为，即当其他线程将来调用 *get()* 或 *wait()* 以访问共享状态时，将调用 *fn*。
      - *launch::async | launch::deferred*：系统默认行为。它可以异步运行或不异步运行，具体取决于系统上的负载。但是我们无法控制它。
    - *fn*：可调用对象，可以是函数、仿函数、*lambda* 表达式、*packaged_task* 等，当可调用对象为 *packaged_task* 时，返回值恒为 *future\<void>*
    - args：可调用对象 *fn* 的输入参数
    - 返回值：future 对象

  - 使用示例

    ```cpp
    int func(int in) {
      this_thread::sleep_for(5s);
      return in + 1;
    }
    // 执行结果：5秒后输出 "value: 6"
    int main() {
      auto fut = async(func, 5);
      cout << fut.get() << endl;
      return 0;
    }
    ```

- 异步任务提供者（*Provider*）

  - *std::async* 函数
  - *std::promise*
  - *std::packaged_task*

- *future_status*：主要用在 *std::future* (或 *std::shared_future*) 中的 *wait_for* 和 *wait_until* 两个函数中

  ```cpp
  enum class future_status { ready, timeout, deferred };
  ```

- *std::launch*：主要用在调用 *std::async* 时设置异步任务的启动策略

  ```cpp
  enum class launch { async = 0x1, deferred = 0x2 };
  ```

- *std::future_error*：继承自 *C++* 标准异常体系中的 *logic_error*

  ```cpp
  class future_error : public logic_error;
  ```

- *std::future_errc*：定义 *std::future_error* 报告的错误代码

  ```cpp
  enum class future_errc {
    broken_promise = 1,           // 与该 std::future 共享状态相关联的 std::promise 对象在设置值或者异常之前已被销毁。
    future_already_retrieved,     // 共享状态的内容已通过 std::future 访问
    promise_already_satisfied,    // 重复设置共享状态的值
    no_state                      // 试图访问没有关联共享状态的 std::promise 或 std::future。
  };
  ```

- *std::future*

  - *std::future* 可以用来获取异步任务的结果

  - *std::future* 通常由某个 *Provider* 创建（*async*、*promise* 或 *packaged_task*）

  - *std::future* 禁用拷贝语义，保留移动语义

  - 成员函数

    ```cpp
    bool valid(); // 检查当前的 std::future 对象是否有效，即是否与某个共享状态相关联。一个有效的 std::future 对象只能通过 std::async(),                     // std::future::get_future 或者 std::packaged_task::get_future 来初始化。
                  // 由 std::future 默认构造函数创建的 std::future 对象是无效(invalid)的。
    T get();      // 通过 get 函数（通常在另外一个线程中） 获取值，如果共享状态的标志不为 ready，则调用 get 会阻塞当前的调用者，直到关联的                     // promise 对象设置共享状态的标志变为 ready，get 返回异步任务的值或异常（如果发生了异常）
    std::shared_future<T> share();// 返回一个 std::shared_future 对象。调用该函数之后，该 std::future 对象本身已经不和任何共享状态相关联，因此                               // 该 std::future 的状态不再是 valid 的了。
    void wait(); // 阻塞等待与当前std::future 对象相关联的共享状态的标志变为 ready。但不读取共享状态的值或异常
    std::future_status wait_for(duration &time);// 与 wait 类似：
                                                // (1)当future对象是从deferred启动的async函数创建时，立即返回 future_status::deferred
                                                // (2)否则，等待 time 指定的时间，未超时则返回 future_status::ready
                                                // (3)超时返回 future_status::timeout
    std::future_status wait_until(time_point &time);// 与 wait_for类似，等待至某一个时刻
    ```

- *std::shared_future*

  - 与 *std::future* 类似，但是 *std::shared_future* 可以拷贝。即 *shared_future* 支持拷贝语义和移动语义

  - 多个 *std::shared_future* 可以共享某个共享状态的最终结果（即共享状态的某个值或者异常）

  - *shared_future* 可以通过某个 *std::future* 对象隐式转换（即存在以 *future* 对象作为输入参数的构造函数），或者通过 *std::future::share()* 显示转换。无论哪种转换，被转换的那个 *std::future* 对象都会变为 *not-valid*。

  - 成员函数

    ```cpp
    bool valid();                                     // 类似于 future 类的 valid
    T get();                                          // 类似于 future 类的 get 
    void wait();                                      // 类似于 future 类的 wait
    std::future_status wait_for(duration &time);      // 类似于 future 类的 wait_for
    std::future_status wait_until(time_point &time);  // 类似于 future 类的 wait_until

#### **std::condition_variable**

- *condition_variable* 需要配合unique_lock 一起使用

- 成员函数

  - ```cpp
    void wait(std::unique_lock<std::mutex> &lck);
    void wait(std::unique_lock<std::mutex> &lck, _Predicate pred); // equals with: while (!pred()) wait(lck);
    ```

    - 调用 *wait* 时，互斥量已经被 *unique_lock* 上锁，此时 *wait* 函数会阻塞本线程，并且将互斥量解锁。
    - 被 *wait* 函数调用阻塞后， 下一次线程被唤醒时，将会尝试对互斥量上锁，随后执行 *wait* 函数之后的代码。若尝试对互斥量上锁而互斥量已经被其他线程锁住时，本线程将再次被阻塞，但阻塞原因是 互斥量的 *lock* 函数调用，而不再是被条件变量阻塞，即线程会从等待信号量的队列中转移到等待互斥锁的队列中。

  - ```cpp
    std::cv_status wait_for(unique_lock<mutex> &lck, const chrono::duration &time);
    bool wait_for(unique_lock<mutex> &lck, const chrono::duration &time, _Predicate pred);
    // equals with:
    // wait_until (lck, chrono::steady_clock::now() + time, std::move(pred));
    ```

    - 类似于 *wait* 函数，当前线程收到通知或者指定的时间 *time* 超时之前，该线程都会处于阻塞状态。
    - 一旦超时或者收到了其他线程的通知，*wait_for* 返回，剩下的处理步骤和 *wait* 类似。
    - 若函数没有超时，返回 *cv_status::no_timeout*，否则返回 *cv_status::timeout*。

  - ```cpp
    std::cv_status wait_until(unique_lock<mutex> &lck, const chrono::time_point &time);
    bool wait_until(unique_lock<mutex> &lck, const chrono::time_point &time, _Predicate pred);
    // equals with:
    /**
     * while (!pred())
     *   if (wait_until(lck, time) == cv_status::timeout) return pred();
     * return true;
     **/
    ```

    - 类似于 *wait* 函数，当前线程收到通知或者未到达指定的时间点 *time* 之前，该线程都会处于阻塞状态。
    - 一旦超时或者收到了其他线程的通知，*wait_until* 返回，剩下的处理步骤和 *wait* 类似。
    - 若函数没有超时，返回 *cv_status::no_timeout*，否则返回 *cv_status::timeout*。

  - ```cpp
    void notify_one();
    void notify_all();
    ```

    - *notify_one*：唤醒某个等待线程。如果当前没有等待线程，则该函数什么也不做，如果同时存在多个等待线程，则唤醒哪个线程是不确定的。该函数的语义是只唤醒一个线程，但操作系统底层的实现上某些场景下该函数唤醒的线程可能不止一个。
    - *notify_all*：唤醒所有的等待线程。如果当前没有等待线程，则该函数什么也不做。

  - ```cpp
    std::condition_variable::native_handle_type native_handle();
    ```

    - 访问 *\*this* 的原生句柄，此函数结果的含义和类型是实现定义的。 **POSIX** 系统上，这可以是 *pthread_cond_t\** 类型值。 **Windows** 系统上，这可以是 *PCONDITION_VARIABLE*。

- *std::condition_variable_any*：与 *std::condition_variable* 类似，只不过 *std::condition_variable_any* 的 *wait* 函数可以接受任何 *lockable* 参数，而 *std::condition_variable* 只能接受 *std::unique_lock\<std::mutex>* 类型的参数，除此以外，和 *std::condition_variable* 几乎完全一样。

- *std::notify_all_at_thread_exit*

  ```cpp
  void notify_all_at_thread_exit (condition_variable& cond, unique_lock<mutex> lck);
  // 当调用该函数的线程退出时，所有在 cond 条件变量上等待的线程都会收到通知。
  ```

- 信号丢失、虚假唤醒与惊群效应

  - ```cpp
    std::mutex mutex;
    std::condition_variable cv;
    std::vector<int> vec;
    
    void Consume() {
      std::unique_lock<std::mutex> lock(mutex);
      cv.wait(lock);
      std::cout << "consume " << vec.size() << "\n";
    }
    void Produce() {
      std::unique_lock<std::mutex> lock(mutex);
      vec.push_back(1);
      cv.notify_all();
      std::cout << "produce \n";
    }
    int main() {
      std::thread t(Consume);
      t.detach();
      Produce();
      return 0;
    }
    ```

    - 上述代码本意是消费者线程阻塞，等待生产者生产数据后去通知消费者线程，这样消费者线程就可以拿到数据去消费。
    - *BUG*：**信号丢失**。如果先执行的 *Produce()*，后执行的 *Consume()*，生产者提前生产出了数据，去通知消费者，但是此时消费者线程如果还没有执行到 *wait* 语句，即线程还没有处于挂起等待状态，线程没有等待此条件变量上，那通知的信号就丢失了， 后面 *Consume()* 中才执行 *wait* 处于等待状态，但此时生产者已经不会再触发 *notify*，那消费者线程就会始终阻塞下去，出现**信号丢失 *bug***。

  - ```cpp
    // 修改 Consume 函数如下，可以解决信号丢失 bug
    void Consume() {
      std::unique_lock<std::mutex> lock(mutex);
      if (vec.empty()) {  // 加入此判断条件
        cv.wait(lock);
      }
      std::cout << "consume " << vec.size() << "\n";
    }
    ```

    - 通过增加附加条件可以解决信号丢失的问题。
    - *BUG*：**虚假唤醒**。处于等待的条件变量可以通过 *notify_one/notify_all* 进行唤醒，调用函数进行信号的唤醒时，处于等待的条件变量会重新进行互斥锁的竞争。没有得到互斥锁的线程就会发生等待转移，从等待信号量的队列中转移到等待互斥锁的队列中，一旦获取到互斥锁的所有权就会接着向下执行，但是此时其他线程已经执行并重置了执行条件，这时该线程执行就可并引发未定义的错误。

  - ```cpp
    // 修改 Consume 函数如下，可以解决虚假唤醒 bug
    void Consume() {
      std::unique_lock<std::mutex> lock(mutex);
      while (vec.empty()) {  // 加入此判断条件
        cv.wait(lock);
      }
      std::cout << "consume " << vec.size() << "\n";
    }
    ```

    - 可以使用 *wait* 的重载函数简化代码：

      ```cpp
      void Consume() {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock, [&]() { return !vec.empty(); });  // 这里可以直接使用 C++的封装
        std::cout << "consume " << vec.size() << "\n";
      }
      ```

  - 惊群效应：多个线程等待一个唤醒的情况叫做惊群效应。

- 使用示例：使用 *condition_variable* 实现简单的信号量

  ```cpp
  #pragma once
  
  #include <functional>
  #include <mutex>
  
  class Semaphore {
   public:
    Semaphore(int count = 0) : count(count){};
  
    void signal() {
      std::unique_lock<std::mutex> lock(mut);
      ++count;
      cv.notify_one();
    }
  
    void wait() {
      std::unique_lock<std::mutex> lock(mut);
      cv.wait(lock, [this] { return count > 0; });
      --count;
    }
  
   private:
    std::mutex mut;
    std::condition_variable cv;
    int count;
  };
  
  auto P = std::mem_fn(&Semaphore::wait);
  auto V = std::mem_fn(&Semaphore::signal);
  ```

#### **std::atomic**

- *std::atomic_flag*：是一种简单的原子布尔类型，只支持两种操作，*test_and_set* 和 *clear*。

  - *std::atomic_flag* 只有默认构造函数，禁用拷贝语义和移动语义

  - 如果在初始化时没有明确使用 *ATOMIC_FLAG_INIT* 初始化，那么新创建的 *std::atomic_flag* 对象的状态是未指定的（既没有被 *set* 也没有被 *clear*）

  - 如果某个 *std::atomic_flag* 对象使用 *ATOMIC_FLAG_INIT* 宏初始化，那么可以保证该 *std::atomic_flag* 对象在创建时处于 *clear* 状态

  - 成员函数 *test_and_set*：

    ```cpp
    bool test_and_set(std::memory_order order = std::memory_order_seq_cst);
    ```

    - *test_and_set()* 函数检查 *std::atomic_flag* 标志，如果 *std::atomic_flag* 之前没有被设置过，则设置 *std::atomic_flag* 的标志，并返回先前该 *std::atomic_flag* 对象是否被设置过，如果之前 *std::atomic_flag* 对象已被设置，则返回 *true*，否则返回 *false*。
    - *test-and-set* 操作是原子的（因此 *test-and-set* 是原子 *read-modify-write* （*RMW*）操作）。
    - test_and_set 可以使用 *order* 参数指定内存序。

  - 成员函数 *clear*：

    ```cpp
    void clear(std::memory_order order = std::memory_order_seq_cst);
    ```

    - *clear* 函数清除 *std::atomic_flag* 对象的标志位，即设置 *atomic_flag* 的值为 *false*。
    - *clear* 函数可以使用 *order* 参数指定内存序。

  - 使用示例：使用 std::atomic_flag 实现简单的自旋锁

    ```cpp
    std::atomic_flag locker = ATOMIC_FLAG_INIT;
    
    void f(int n) {
      for (int cnt = 0; cnt < 100; ++cnt) {
        while (locker.test_and_set(std::memory_order_acquire))  // acquire lock
          ;                                                     // spin
        std::cout << "Output from thread " << n << '\n';
        locker.clear(std::memory_order_release);  // release lock
      }
    }
    
    int main() {
      std::vector<std::thread> v;
      for (int n = 0; n < 10; ++n) v.emplace_back(f, n);
      for (auto& t : v) t.join();
    }
    ```

- *std::atomic*

  - *std::atomic* 是模板类，一个模板类型为 *T* 的原子对象中封装了一个类型为 *T* 的值。

  - *C++11* 标准库 *std::atomic* 提供了针对整形 (*integral*) 和指针类型的特化实现

  - 构造函数

    ```cpp
    atomic() noexcept = default;       // 默认构造函数，对象处于未初始化状态，未初始化的 atomic 对象可以使用 atomic_init 函数进行初始化
    atomic(T val);                     // 初始化构造函数，由类型 T初始化一个 std::atomic对象。
    atomic(const atomic&) = delete;    // 禁止拷贝构造，且禁止拷贝赋值
    atomic(atomic&&) = delete;         // 禁止移动构造，且禁止移动赋值
    ```

  - 成员函数

    - ```cpp
      T operator= (T val);
      ```

      赋值操作符，允许使用模板类型 *T* 的变量赋值给 *atomic\<T>* 类型原子变量。

    - ```cpp
      bool is_lock_free();
      ```

      判断该 *std::atomic* 对象是否具备 *lock-free* 的特性。

    - ```cpp
      void store(int value, std::memory_order order = std::memory_order_seq_cst);
      ```

      修改被封装的值，可以指定内存序。

    - ```cpp
      T load(memory_order order = memory_order_seq_cst);
      ```

      读取被封装的值，可以指定内存序。

    - ```cpp
      operator T() const;
      ```

      与 *load* 功能类似，也是读取被封装的值，*operator T()* 是类型转换操作。

    - ```cpp
      T exchange(T val, memory_order order = memory_order_seq_cst);
      ```

      读取并修改被封装的值，*exchange* 会使用 *val* 替换掉之前该原子对象封装的值，并返回之前该原子对象封装的值，整个过程是原子的 (因此 *exchange* 操作也称为 *read-modify-write* 操作)。*order* 参数指定内存序。

    - ```cpp
      bool compare_exchange_weak(T& expected, T val, memory_order order = memory_order_seq_cst) noexcept;
      bool compare_exchange_weak(T& expected, T val, memory_order success, memory_order failure) noexcept;
      ```

      - 比较并交换被封装的值与参数 *expected* 所指定的值是否相等，原子操作。

        - 如果相等，则用 *val* 替换原子对象的旧值。

        - 如果不相等，则用原子对象的旧值替换 *expected*。

      - 该函数直接比较原子对象所封装的值与参数 *expected* 的物理内容，不等价于调用 *operator==()*。

      - 该函数可能发生伪失败：正常情况下，比较物理内容相等时会返回 *true*，不相等时会返回 *false*，但某些时候比较物理内容相等时也会返回 *false*，此时函数返回 *false*，并且参数 *expected* 的值不会改变。伪失败的解决方法是循环调用该函数直至不再出现伪失败。

      - 该函数在一些平台下性能优于 *compare_exchange_strong* 函数。如果算法本身需要循环操作来做检查， *compare_exchange_weak* 的性能会优于 *compare_exchange_strong*。
      - *order* 指定内存序，*success* 指定比较物理内容相等时的内存序，*failure* 指定比较物理内容不相等时的内存序。

    - ```cpp
      bool compare_exchange_strong(T& expected, T val, memory_order order = memory_order_seq_cst) noexcept;
      bool compare_exchange_strong(T& expected, T val, memory_order success, memory_order failure) noexcept;
      ```

      - 与 *compare_exchange_weak* 类似，但保证不会产生伪失败，比较结果相同则返回 *true*，否则返回 *false*。
      - 对于某些不需要采用循环操作的算法而言, 通常采用 *compare_exchange_strong* 优于 *compare_exchange_weak*。

  - 使用示例：线程安全的不带头结点的单链表

    ```cpp
    struct Node {
      int value;
      Node* next;
    };
    
    std::atomic<Node*> list_head(nullptr);
    
    void append(int val) {
      // 头插法追加元素
      Node* newNode = new Node{val, list_head};
      while (!(list_head.compare_exchange_weak(newNode->next, newNode)))
        ;
    }
    
    int main() {
      // spawn 10 threads to fill the linked list:
      std::vector<std::thread> threads;
      for (int i = 0; i < 10; ++i) threads.push_back(std::thread(append, i));
      for (auto& th : threads) th.join();
    
      // print contents:
      for (Node* it = list_head; it != nullptr; it = it->next)
        std::cout << ' ' << it->value;
    
      std::cout << '\n';
    
      // cleanup:
      Node* it;
      while (it = list_head) {
        list_head = it->next;
        delete it;
      }
    
      return 0;
    }
    ```

- *std::atomic* 针对整型和指针类型的特化版本新增的函数

  - ```cpp
    T fetch_add(T val, memory_order order = memory_order_seq_cst);	// if T is integeral
    T fetch_add(T val, memory_order order = memory_order_seq_cst); // if T is pointer
    ```

    将原子对象的封装值加 *val*，并返回原子对象的旧值（适用于整形和指针类型的 *std::atomic* 特化版本），整个过程是原子的。*order* 参数指定内存序。

  - ```cpp
    T fetch_sub(T val, memory_order order = memory_order_seq_cst);	// if T is integeral
    T fetch_sub(T val, memory_order order = memory_order_seq_cst); // if T is pointer
    ```

    类似于 *fetch_add*，执行减法操作。

  - ```cpp
    T fetch_and(T val, memory_order order = memory_order_seq_cst);
    ```

    将原子对象的封装值按位与 *val*，并返回原子对象的旧值（只适用于整型的 *std::atomic* 特化版本），整个过程是原子的。*order* 参数指定内存序。

  - ```cpp
    T fetch_or (T val, memory_order order = memory_order_seq_cst);
    ```

    类似于 *fetch_and*，执行按位或操作。

  - ```cpp
    T fetch_or (T val, memory_order order = memory_order_seq_cst);
    ```

    类似于 *fetch_and*，执行按位异或操作。

  - ```cpp
    T operator++();
    T operator++ (int);
    ```

    自增运算符，适用于整形和指针类型的 *std::atomic* 特化版本。

  - ```cpp
    T operator--();
    T operator-- (int);
    ```

    自减运算符，适用于整形和指针类型的 *std::atomic* 特化版本。

  - ```cpp
    // if T is integral
    T operator+= (T val) volatile noexcept;
    T operator+= (T val) noexcept;
    T operator-= (T val) volatile noexcept;
    T operator-= (T val) noexcept;
    T operator&= (T val) volatile noexcept;
    T operator&= (T val) noexcept;
    T operator|= (T val) volatile noexcept;
    T operator|= (T val) noexcept;
    T operator^= (T val) volatile noexcept;
    T operator^= (T val) noexcept;
    
    // if T is pointer	
    T operator+= (ptrdiff_t val) volatile noexcept;
    T operator+= (ptrdiff_t val) noexcept;
    T operator-= (ptrdiff_t val) volatile noexcept;
    T operator-= (ptrdiff_t val) noexcept;
    ```

    复合赋值运算符，都有对应的 *fetch* 操作，内存序为默认内存序。

#### 并行算法库

- 从C++17开始。\<algorithm> 和 \<numeric> 头文件的中的很多算法都添加了一个新的参数：*sequenced_policy*。借助这个参数，开发者可以直接使用这些算法的并行版本，不用再自己创建并发系统和划分数据来调度这些算法。

- ```cpp
  std::vector<int> my_data;
  std::sort(std::execution::par,my_data.begin(),my_data.end());
  ```

#### 乐观锁

- 每次去拿数据的时候都认为别人不会修改。所以不会上锁，但是如果想要更新数据，则会在更新前检查在读取至更新这段时间别人有没有修改过这个数据。如果修改过，则重新读取，再次尝试更新，循环上述步骤直到更新成功（当然也允许更新失败的线程放弃操作）。

- 乐观锁回滚重试。适用于写比较少的情况下，即冲突真的很少发生的时候，这样可以省去锁的开销，加大了系统的整个吞吐量。

- 实现方式一：版本号机制

  - 版本号机制是在数据表中加上一个 `version` 字段来实现的，表示数据被修改的次数。当执行写操作并且写入成功后，version = version + 1，当线程A要更新数据时，在读取数据的同时也会读取 version 值，在提交更新时，若刚才读取到的 version 值为当前数据库中的version值相等时才更新，否则重试更新操作，直到更新成功。
  - 实例：
    - 存款 w = 100元，版本号 v = 0。
    - 线程 A 准备取款 20 元，线程 B 准备取款 10 元。正常情况下结果：w = 70，v = 2。
    - 线程 A 读取：moneyInA = w，versionInA = v。此时 moneyInA 为 100，versionInA 为 0。
    - 接着线程 B 读取：moneyInB = w，versionInB = v。此时 moneyInB 为 100，versionInB 为 0。
    - 线程 A 取款：moneyInA -= 20，versionInA += 1，此时 moneyInA 为 80，versionInA 为 1。
    - 接着线程 B 取款：moneyInB -= 10，versionInB += 1，此时 moneyInB 为 90，versionInB 为 1。
    - 线程 A 提交事务：此时 v = 0，而 versionInA = 1，versionInA == v + 1，事务提交成功，v = versionInA，w = moneyInA，即 v = 1，w = 80。
    - 接着线程 B 提交事务：此时 v = 1，而 versionInB = 1，versionInB != v + 1，事务提交失败。
    - 接着线程 B 重新读取：moneyInB = w，versionInB = v。此时 moneyInB 为 80，versionInB 为 1。
    - 接着线程 B 重新取款：moneyInB -= 10，versionInB += 1，此时 moneyInB 为 70，versionInB 为 2。
    - 线程 B 重新提交事务：此时 v = 1，而 versionInB = 2，versionInB == v + 1，事务提交成功，v = versionInB，w = moneyInB，即 v = 2，w = 70。
    - 注意：事务提交操作 "v = versionInA，w = moneyInA"，"v = versionInB，w = moneyInB" 必须是原子操作。

- 实现方式二：CAS

  - compare_and_swap（CAS）：

  - GCC4.1+中 CAS api：

    ```c
    // CAS，比较 ptr 指向的内容与 oldval 是否相同。若相同，执行 *ptr = newval，并返回 1，否则什么也不做，并返回 0。
    bool __sync_bool_compare_and_swap (type *ptr, type oldval, type newval);
    // CAS，比较 ptr 指向的内容与 oldval 是否相同。若相同，执行 *ptr = newval，并返回 ptr 指向的旧值，否则什么也不做，并返回 ptr 指向的旧值。
    type __sync_val_compare_and_swap (type *ptr, type oldval, type newval);
    ```

  - c++11 的语法支持为 compare_exchange_strong 和 compare_exchange_weak。

- ABA问题：如果一个变量第一次读取的值是 A，准备好需要对 A 进行写操作的时候，发现值还是 A，此时可能已经发生了 A -> B -> A 的转换，但是原子变量的CAS操作无法看到这个变化，此时可以结合版本号机制，解决ABA问题。

#### 悲观锁

- 每次去拿数据的时候都认为别人会修改。所以每次在拿数据的时候都会上锁。这样别人想拿数据就被挡住，直到悲观锁被释放。
- 悲观锁阻塞事务，适用于多写场景。

#### 自旋锁

- 当一个线程尝试去获取某一把锁的时候，如果这个锁此时已经被别人获取(占用)，那么此线程就无法获取到这把锁，该线程将会等待，间隔一段时间后会再次尝试获取。这种采用循环尝试加锁 -> 等待的机制被称为自旋锁。

- 自旋锁尽可能的减少线程的阻塞，这对于锁的竞争不激烈，且占用锁时间非常短的代码块来说性能能大幅度的提升，因为自旋的消耗会小于线程阻塞挂起再唤醒的操作的消耗，这些操作会导致线程发生两次上下文切换。

- 但是如果锁的竞争激烈，或者持有锁的线程需要长时间占用锁执行同步块，这时候就不适合使用自旋锁了，因为自旋锁在获取锁前一直都是占用 cpu 做无用功，同时有大量线程在竞争一个锁，会导致获取锁的时间很长，线程自旋的消耗大于线程阻塞挂起操作的消耗，其它需要 cpu 的线程又不能获取到 cpu，造成 cpu 的浪费。

- 自旋锁简单实现：

  - 实现一：raw_spinlock

    ```cpp
    class raw_spinlock {
     public:
      raw_spinlock() noexcept {};
      void lock() noexcept {
        while (flag.test_and_set(std::memory_order_acquire))
          ;
      }
      void unlock() noexcept { flag.clear(std::memory_order_release); }
      bool try_lock() noexcept {
        return !flag.test_and_set(std::memory_order_acquire);
      }
    
     private:
      std::atomic_flag flag = ATOMIC_FLAG_INIT;
    };
    ```

  - 实现二：ticket_spinlock

    ```cpp
    class ticket_spinlock {
     public:
      ticket_spinlock() : head(0), tail(0){};
      void lock() {
        std::atomic<int> ticket(tail.fetch_add(1));
        while (ticket != head)
          ;
        owner = std::this_thread::get_id();
      }
      void unlock() {
        if (std::this_thread::get_id() != owner) return;
        ++head;
      }
    
     private:
      std::atomic<int> head;
      std::atomic<int> tail;
      std::thread::id owner;
    };
    ```

  - 实现三：msc_spinlock（Java）

    ```java
    public class MCSLock {
    
        public static class MCSNode {
            volatile MCSNode next;
            volatile boolean isLocked = true;
        }
    
        private static final ThreadLocal<MCSNode> NODE = new ThreadLocal<>();
    
        // 队列
        @SuppressWarnings("unused")
        private volatile MCSNode queue;
    
        private static final AtomicReferenceFieldUpdater<MCSLock,MCSNode> UPDATE =
                AtomicReferenceFieldUpdater.newUpdater(MCSLock.class,MCSNode.class,"queue");
    
        public void lock(){
            // 创建节点并保存到ThreadLocal中
            MCSNode currentNode = new MCSNode();
            NODE.set(currentNode);
    
            // 将queue设置为当前节点，并且返回之前的节点
            MCSNode preNode = UPDATE.getAndSet(this, currentNode);
            if (preNode != null) {
                // 如果之前节点不为null，表示锁已经被其他线程持有
                preNode.next = currentNode;
                // 循环判断，直到当前节点的锁标志位为false
                while (currentNode.isLocked) {
                }
            }
        }
    
        public void unlock() {
            MCSNode currentNode = NODE.get();
            // next为null表示没有正在等待获取锁的线程
            if (currentNode.next == null) {
                // 更新状态并设置queue为null
                if (UPDATE.compareAndSet(this, currentNode, null)) {
                    // 如果成功了，表示queue==currentNode,即当前节点后面没有节点了
                    return;
                } else {
                    // 如果不成功，表示queue!=currentNode,即当前节点后面多了一个节点，表示有线程在等待
                    // 如果当前节点的后续节点为null，则需要等待其不为null（参考加锁方法）
                    while (currentNode.next == null) {
                    }
                }
            } else {
                // 如果不为null，表示有线程在等待获取锁，此时将等待线程对应的节点锁状态更新为false，同时将当前线程的后继节点设为null
                currentNode.next.isLocked = false;
                currentNode.next = null;
            }
        }
    }
    ```

  - 实现四：qspinlock（Java）

    ```java
    public class CLHLock {
    
        public static class CLHNode{
            private volatile boolean isLocked = true;
        }
    
        // 尾部节点
        private volatile CLHNode tail;
        private static final ThreadLocal<CLHNode> LOCAL = new ThreadLocal<>();
        private static final AtomicReferenceFieldUpdater<CLHLock,CLHNode> UPDATER =
                AtomicReferenceFieldUpdater.newUpdater(CLHLock.class,CLHNode.class,"tail");
    
    
        public void lock(){
            // 新建节点并将节点与当前线程保存起来
            CLHNode node = new CLHNode();
            LOCAL.set(node);
    
            // 将新建的节点设置为尾部节点，并返回旧的节点（原子操作），这里旧的节点实际上就是当前节点的前驱节点
            CLHNode preNode = UPDATER.getAndSet(this,node);
            if(preNode != null){
                // 前驱节点不为null表示当锁被其他线程占用，通过不断轮询判断前驱节点的锁标志位等待前驱节点释放锁
                while (preNode.isLocked){
    
                }
                preNode = null;
                LOCAL.set(node);
            }
            // 如果不存在前驱节点，表示该锁没有被其他线程占用，则当前线程获得锁
        }
    
        public void unlock() {
            // 获取当前线程对应的节点
            CLHNode node = LOCAL.get();
            // 如果tail节点等于node，则将tail节点更新为null，同时将node的lock状态职位false，表示当前线程释放了锁
            if (!UPDATER.compareAndSet(this, node, null)) {
                node.isLocked = false;
            }
            node = null;
        }
    }
    ```

    
