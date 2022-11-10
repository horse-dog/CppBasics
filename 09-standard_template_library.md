#### 分配器

- 分配器(allocator))是C ++标准库的一个组件, 主要用来处理所有给定容器(vector，list，map等)内存的分配和释放。

- C ++标准库提供了默认使用的通用分配器std::allocator，但开发者可以自定义分配器。

- GNU STL除了提供默认分配器，还提供了\__pool_alloc、__mt_alloc、array_allocator、malloc_allocator 内存分配器。

  - __pool_alloc ：SGI内存池分配器。

  - __mt_alloc ： 多线程内存池分配器。

  - array_allocator ： 全局内存分配，只分配不释放，交给系统来释放。

  - malloc_allocator ：对 std::malloc 和 std::free 进行的封装。

- 大多数情况下，STL默认的allocator就已经足够了。这个allocator是一个由两级分配器构成的内存管理器，当申请的内存大小大于128byte时，就启动第一级分配器通过malloc直接向系统的堆空间分配，如果申请的内存大小小于128byte时，就启动第二级分配器，从一个预先分配好的内存池中取一块内存交付给用户，这个内存池由16个不同大小（8的倍数，8~128byte）的空闲列表组成，allocator会根据申请内存的大小（将这个大小round up成8的倍数）从对应的空闲块列表取表头块给用户。

  ![](.\img\image-20220716151831992.png)

- 自定义 allocator 只需要实现 allocate 和 deallocate，来实现自己的内存分配策略。

  ```cpp
  template <typename T>
  class myalloctor {
   public:
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef const T* const_pointer;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    template <typename U>
    struct rebind {
      typedef myalloctor<U> other;
    };
  
    myalloctor() = default;
    ~myalloctor() = default;
    pointer address(reference x) { return static_cast<pointer>(&x); }
    const_pointer address(const_reference x) {
      return static_cast<const_pointer>(&x);
    }
  
    pointer allocate(size_type n, const void* hit = 0) {
      std::cout << "allocate" << std::endl;
      return static_cast<T*>(operator new(n * sizeof(T)));
    }
  
    void deallocate(pointer p, size_type n) {
      std::cout << "deallocate" << std::endl;
      operator delete(p);
    }
  
    size_type max_size() const throw() {
      return static_cast<size_type>(INT_MAX / sizeof(T));
    }
  
    void construct(pointer p, const_reference value) { new (p) T(value); }
  
    void destroy(pointer p) { p->~T(); }
  };
  ```

#### 迭代器

- 迭代器 iterator 是C++ STL的组件之一，作用是用来遍历容器，而且是通用的遍历容器元素的方式。

- 无论容器是基于什么数据结构实现的，尽管不同的数据结构，遍历元素的方式不一样，但是用迭代器遍历不同容器的代码是完全一样的。

- 基本的迭代器需要实现的操作符

  ```cpp
  operator++();
  operator--();
  operator*();
  operator->();
  bool operator==(const iterator &iter);
  bool operator!=(const iterator &iter);
  ```

- 继承关系

  ```cpp
  class const_iterator;
  class iterator : public const_iterator;
  class const_reverse_iterator;
  class reverse_iterator : public const_reverse_iterator;
  ```
  
- 链表的迭代器实现：

  ```cpp
  template <typename T>
  class linklist {
   private:
    DLinkNode<T> *head = nullptr;  // head node
    size_t length = 0;             // length of list
  
   public:
    class list_const_iterator {
      friend class linklist<T>;
      using difference_type = ptrdiff_t;
  
     protected:
      DLinkNode<T> *node;
  
     public:
      list_const_iterator() : node(nullptr) {}
  
     protected:
      explicit list_const_iterator(DLinkNode<T> *const ptr) : node(ptr) {}
  
     public:
      const T &operator*() const { return node->data; }
      const T *operator->() const { return &node->data; }
      list_const_iterator &operator++() {
        node = node->next;
        return *this;
      }
      list_const_iterator operator++(int) {
        list_const_iterator old(this->node);
        ++(*this);
        return old;
      }
      list_const_iterator &operator--() {
        node = node->prev;
        return *this;
      }
      list_const_iterator operator--(int) {
        list_const_iterator old(this->node);
        --(*this);
        return old;
      }
      bool operator==(const list_const_iterator &iter) const {
        return node == iter.node;
      }
      bool operator!=(const list_const_iterator &iter) const {
        return node != iter.node;
      }
    };
  
    class list_iterator : public list_const_iterator {
      friend class linklist<T>;
      using super = list_const_iterator;
      using difference_type = ptrdiff_t;
  
     public:
      list_iterator() {}
  
     protected:
      explicit list_iterator(DLinkNode<T> *ptr) : super(ptr) {}
  
     public:
      T &operator*() const { return const_cast<T &>(super::operator*()); }
      T *operator->() const { return const_cast<T *>(super::operator->()); }
      list_iterator &operator++() {
        ++*(super *)this;
        return *this;
      }
      list_iterator operator++(int) {
        list_iterator old = *this;
        ++(*this);
        return old;
      }
      list_iterator &operator--() {
        --*(super *)this;
        return *this;
      }
      list_iterator operator--(int) {
        list_iterator old = *this;
        --(*this);
        return old;
      }
    };
  
    class list_const_reverse_iterator {
      friend class linklist<T>;
      using difference_type = ptrdiff_t;
  
     protected:
      DLinkNode<T> *node;
  
     public:
      list_const_reverse_iterator() : node(nullptr) {}
  
     protected:
      explicit list_const_reverse_iterator(DLinkNode<T> *const ptr) : node(ptr) {}
  
     public:
      const T &operator*() const { return node->data; }
      const T *operator->() const { return &node->data; }
      list_const_reverse_iterator &operator++() {
        node = node->prev;
        return *this;
      }
      list_const_reverse_iterator operator++(int) {
        list_const_reverse_iterator old(this->node);
        ++(*this);
        return old;
      }
      list_const_reverse_iterator &operator--() {
        node = node->next;
        return *this;
      }
      list_const_reverse_iterator operator--(int) {
        list_const_reverse_iterator old(this->node);
        --(*this);
        return old;
      }
      bool operator==(const list_const_reverse_iterator &iter) const {
        return node == iter.node;
      }
      bool operator!=(const list_const_reverse_iterator &iter) const {
        return node != iter.node;
      }
    };
  
    class list_reverse_iterator : public list_const_reverse_iterator {
      friend class linklist<T>;
      using super = list_const_reverse_iterator;
      using difference_type = ptrdiff_t;
  
     public:
      list_reverse_iterator() {}
  
     protected:
      explicit list_reverse_iterator(DLinkNode<T> *ptr) : super(ptr) {}
  
     public:
      T &operator*() const { return const_cast<T &>(super::operator*()); }
      T *operator->() const { return const_cast<T *>(super::operator->()); }
      list_reverse_iterator &operator++() {
        ++*(super *)this;
        return *this;
      }
      list_reverse_iterator operator++(int) {
        list_reverse_iterator old = *this;
        ++(*this);
        return old;
      }
      list_reverse_iterator &operator--() {
        --*(super *)this;
        return *this;
      }
      list_reverse_iterator operator--(int) {
        list_reverse_iterator old = *this;
        --(*this);
        return old;
      }
    };
  
    using iterator = list_iterator;
    using const_iterator = list_const_iterator;
    using reverse_iterator = list_reverse_iterator;
    using const_reverse_iterator = list_const_reverse_iterator;
  };
  ```

#### 插入器

|         插入器          |                             功能                             |
| :---------------------: | :----------------------------------------------------------: |
| *back_insert_iterator*  | 在指定容器的尾部插入新元素，前提是必须有 push_back() 成员方法的容器（包括 vector、deque 和 list）。 |
| *front_insert_iterator* | 在指定容器的头部插入新元素，前提是必须有 push_front() 成员方法的容器(list、deque 和 forward_list)。需要包含头文件 \<iterator> |
|    *insert_iterator*    | 在容器的指定位置之前插入新元素，前提是该容器必须提供有 insert() 成员方法。需要包含头文件 \<iterator> |

- 插入器构造需要指定容器对象

  ```cpp
  vector<int> v = {1, 2};
  back_insert_iterator<vector<int>> back_it(v);
  ```

- 插入器重载了赋值运算符

  ```cpp
  vector<int> v = {1, 2};
  back_insert_iterator<vector<int>> back_it(v);
  for (int i = 3; i < 5; i++) back_it = i;      // v = {1, 2, 3, 4}
  ```

|       函数       |                    功能                     |
| :--------------: | :-----------------------------------------: |
| back_inserter()  | 创建 back_insert_iterator 类型的插入迭代器  |
| front_inserter() | 创建 front_insert_iterator 类型的插入迭代器 |
|    inserter()    |    创建 insert_iterator 类型的插入迭代器    |

```cpp
list<int> l = {1, 2};
auto biit = std::back_inserter(l);             // equal with: back_insert_iterator<list<int>> biit(l);
auto fiit = std::front_inserter(l);            // equal with: front_insert_iterator<list<int>> fiit(l);
auto iit  = std::back_inserter(l, l.begin());  // equal with: insert_iterator<list<int>> iit(l, l.begin());
```

#### 顺序容器 std::vector：动态数组

- 模板原型

  ```cpp
  template<typename T, typename Alloc = std::allocator<T>> class vector;
  ```

  - T：数据类型
  - Alloc：内存分配器

- 成员函数

  - ```cpp
    // 重新初始化 vector
    void assign(std::initializer_list<int> l);
    void assign(std::size_t n, const int &val);
    void assign(InputIterator first, InputIterator last);
    ```
  
  - ```cpp
    // operator[]
    T &at(size_t index);
    const T &at(size_t index) const;
    ```
  
  - ```cpp
    // 取首尾元素
    T &front();
    T &back();
    const T &front() const;
    const T &back() const;
    ```
  
  - ```cpp
    // 获取数组指针
    T *data();
    const T *data() const;
    ```
  
  - ```cpp
    // 获取迭代器
    begin(); cbegin(); rbegin(); crbegin();
    end(); cend(); rend(); crend();
    ```
  
  - ```cpp
    size_t size() const;	 // 获取数组长度
    size_t capacity() const; // 获取容量
    bool empty() const;      // 是否为空
    ```
  
  - ```cpp
    // 插入元素
    iterator insert(const_iterator where, const T &val);
    iterator insert(const_iterator where, size_t count, const T &val);
    iterator insert(const_iterator where, std::initializer_list<T> list);
    
    // 插入元素，支持采用T的参数直接构造
    emplace<Args...>(std::vector<T>::const_iterator where, Args &&...args);
    ```
  
  - ```cpp
    void push_back(const T &val);     // 追加元素
    void emplace_back(const T &val);  // 追加元素，支持采用T的参数直接构造
    void pop_back();                  // 弹出元素
    ```
  
  - ```cpp
    // 删除元素
    iterator erase(const_iterator where);
    iterator erase(const_iterator from, const_iterator to);
    ```
  
  - ```cpp
    void clear();                                // 清空vector: 设置数组长度为0，不回收内存
    void resize(size_t newsize);                 // 重设vector长度
    void resize(size_t newsize, const T &val);   // 重设vector长度，多出的位置采用val初始化
    void reserve(size_t newcapacity);            // 重设vector容量
    void shrink_to_fit();                        // 使容量与长度匹配
    void swap(vector<T> &right)                  // 交换vector
    allocator_type get_allocator();              // 获取分配器
    size_t max_size();                           // 获取vector容器支持的最大容量
    ```

#### 顺序容器 std::list：双向链表

- 模板原型

  ```cpp
  template<typename T, typename Alloc = std::allocator<T>> class list;
  ```

- 成员函数

  - ```cpp
    // 相比vector新增了以下函数:
    // 归并两个有序链表，right将被置空
    void merge(list<T> &right)；
    void merge(list<T> &right, std::function<bool(const T &, const T &)> pred);
    
    // 头部增删元素
    void push_front();
    void emplace_front();
    void pop_front();
    
    // 删除指定值 / 指定条件的元素
    void remove(const T &val);
    void remove_if(std::function<bool(const T &)> pred);
    
    // 反转链表
    void reverse();
    // 排序：改变结点链接而非改变结点数据
    void sort();
    
    // 将right链表插入到指定位置，right链表将被置为空
    void splice(const_iterator where, linklist<T> &right);
    
    // 将right链表的from迭代器指向的结点插入到指定位置，right链表将移除from迭代器指向的结点
    void splice(const_iterator where, linklist<T> &right, const_iterator from);
    
    // 将right链表的from迭代器到to迭代器之间的结点插入到指定位置，right链表将移除该区间结点
    void splice(const_iterator where, linklist<T> &right, const_iterator from, const_iterator to);
    
    // 从链表中删除所有连续重复的元素
    void unique();
    void unique(std::function<bool(const T &, const T &)> pred);
    ```

  - ```cpp
    // 相比vector删除了以下函数:
    
    size_t capacity() const;
    void reserve(size_t newcapacity);
    void shrink_to_fit();
    ```

#### 顺序容器 std::forward_list：单链表

- 模板原型

  ```cpp
  template<typename T, typename Alloc = allocator<T>> class forward_list;
  ```

- forward_list 没有实现反向迭代器。

- 成员函数

  - ```cpp
    // 相比list新增了以下函数:
    
    // 返回指向第一个元素之前的迭代器
    iterator before_begin();
    iterator cbefore_begin();
    
    // 在指定位置之后插入一个新元素，并返回一个指向新元素的迭代器。
    insert_after();
    emplace_after();
    
    // 删除容器中指定位置的后一个元素
    erase_after(it);
    
    // 删除容器中指定范围的元素：[it1++, it2)
    erase_after(it1, it2);
    
    // 与 list 的 splice 函数类似，但操作指定迭代器的后一个元素
    splice_after();
    ```

  - ```cpp
    // 相比list删除了以下函数:
    rbegin(); crbegin(); rend(); crend();             // 反向迭代器相关函数
    insert(); emplace(); erase(); splice();           // 替换为了 insert_after(); emplace_after(); erase_after(); splice_after();
    back(); push_back(); emplace_back(); pop_back();  // 删除了尾部操作函数，因为查询尾部元素需要 O(n) 复杂度
    ```

#### 顺序容器 deque：双端队列

- 模板原型

  ```cpp
  template<typename T, typename Alloc = std::allocator<T>> class deque;
  ```

- 成员函数

  ```cpp
  // 相比vector增加了以下函数:
  void push_front();
  void emplace_front();
  void pop_front();
  ```

#### 容器适配器 stack：栈

- 模板原型

  ```cpp
  template<typename Tp, typename Sequence = deque<T>> class stack;
  ```

- stack 是容器适配器，可以指定底层容器，需要满足 empty、size、back、push_back 和 pop_back 操作。默认容器为 deque。

  - 若使用 vector 作为底层容器，扩容时可能复杂度较高。
  - 若使用 list 作为底层容器，每次的压入弹出开销较大，但很平稳。

- 成员函数

  ```cpp
  bool empty();                 // 判空
  size_t size();                // 获取长度
  void emplace(Args&&... args); // 入栈
  void push(const T &val);      // 入栈  
  void pop();                   // 出栈
  void swap(stack<T> &right);   // 交换
  T &top();                     // 获取栈顶
  ```

#### 容器适配器 queue：队列

- 模板原型

  ```cpp
  template<typename T, typename Sequence = deque<T>> class queue;
  ```

- queue 是容器适配器，可以指定底层容器，需要满足 empty、size、front、back、push_back 和 pop_front 操作。默认容器为 deque，可以为 list。

- 成员函数

  ```cpp
  bool empty();                 // 判空
  size_t size();                // 获取长度
  T &front();                   // 获取队首元素
  T &back();                    // 获取队尾元素
  void emplace(Args&&... args); // 入队
  void push(const T &val);      // 入队
  void pop();                   // 出队
  void swap(queue<T> &right);   // 交换
  ```

#### 容器适配器 priority_queue：优先队列

- 模板原型

  ```cpp
  template<typename T, typename Sequence = vector<T>, typename Compare  = less<typename Sequence::value_type>> class priority_queue;
  ```

- priority_queue 会将入队的元素安装优先顺序排列，队首元素优先级最高，默认为 less 比较的较大者优先级较高。

- priority_queue 是容器适配器，可以指定底层容器，需要满足 empty、size、front、push_back 和 pop_back 操作。默认容器为 vector，可以为 list 和 deque。

- 成员函数

  ```cpp
  bool empty();                           // 判空
  size_t size();                          // 获取长度
  void emplace(Args&&... args);           // 入队
  void push(const T &val);                // 入队  
  void pop();                             // 出队
  void swap(priority_queue <T> &right);   // 交换
  T &top();                               // 获取栈顶
  ```

#### 关联容器 set、multiset、unordered_set、unordered_multiset、hash_set、hash_multiset

- multi 允许存储重复元素，unordered 使用哈希表实现，非 unordered 使用红黑树实现。

- 模板原型

  ```cpp
  template<class T, class Pr = less<T>, class Alloc = allocator<T>> class set;
  template<class T, class Pr = less<T>, class Alloc = allocator<T>> class multiset;
  template<class T, class Hasher = hash<T>, class Eq = equal_to<T>, class Alloc = allocator<T>> class unordered_set;
  template<class T, class Hasher = hash<T>, class Eq = equal_to<T>, class Alloc = allocator<T>> class unordered_multiset;
  template<class T, class Tr = hash_compare<T, less<T>>, class Alloc = allocator<T>> class hash_set;
  template<class T, class Tr = hash_compare<T, less<T>>, class Alloc = allocator<T>> class hash_multiset;
  ```

- unordered_set、unordered_multiset、hash_set、hash_multiset 不支持反向迭代器。

- hash_set、hash_multiset 与 unordered_set、unordered_multiset 接口一样，均使用哈希表实现，但效率更低，不建议使用。

- 成员函数

  ```cpp
  // 公共部分
  begin(); cbegin(); end(); cend();
  void clear(); void empty(); void swap();
  size_t size(); size_t max_size(); 
  size_t count(val);                        // 返回元素出现次数
  insert(); emplace(); emplace_hint();      // 插入元素
  iterator lower_bound(val);                // 返回第一个大于或等于 val 的元素的迭代器。
  iterator upper_bound(val);                // 返回第一个大于 val 的元素的迭代器。
  pair<it, it> equal_range(val) const;      // 返回一个 pair 对象（包含 2 个迭代器），该范围中包含值为 val 的元素。
  iterator erase();                         // 删除指定值 / 指定位置 / 指定范围内的元素
  node_type extract();                      // 删除指定值 / 指定位置的结点，并返回该结点的拷贝
  iterator find();                          // 寻找指定值的结点，返回其迭代器
  allocator_type get_allocator() const;     // 返回内存分配器
  key_compare key_comp() const;             // 返回键比较函数，在set 系列中与 value_comp 等价
  value_compare value_comp() const;         // 返回值比较函数，在set 系列中与 key_comp 等价
  void merge(other);                        // 与other合并，对于非 multi 类型 set，多余的重复元素会保留在 other 中，否则other为空
  
  // set、multiset 独有函数
  rbegin(); crbegin(); rend(); crend();     // set、multiset
  
  // unordered_set、unordered_multiset、hash_set、hash_multiset 独有函数
  size_t bucket(val);                       // 返回值为 val 的元素所在桶的编号
  size_t bucket_count();                    // 返回当前容器底层存储元素时，使用桶（一个线性链表代表一个桶）的数量
  size_t bucket_size();                     // 返回第 n 个桶中存储元素的数量
  hasher hash_function() const;             // 返回哈希函数
  key_equal key_eq() const;                 // 返回等价函数
  float load_factor() const;                // 计算当前的负载因子，load_factor = size / bucket_count
  size_t max_bucket_count() const;          // 返回当前系统中，unordered_set / unordered_multiset 容器底层最多可以使用多少桶
  void max_load_factor(float fc);           // 设置新的负载因子阈值
  float max_load_factor();                  // 返回负载因子阈值
  void rehash(size_t n);                    // 将当前容器底层使用桶的数量设置为n，如果n小于当前桶数量，则什么都不做
  void reserve(size_t n);                   // 将桶的数量设置为至少容纳count个元素（不超过最大负载因子）所需的数量，并重新整理容器
  ```

#### 关联容器 map、multimap、unordered_map、unordered_multimap、hash_map、hash_multimap

- multi 允许存储重复键，unordered 使用哈希表实现，非 unordered 使用红黑树实现。

- 模板原型

  ```cpp
  template<class K, class V, class Pr = less<K>, class Alloc = allocator<pair<const K, V>>> class map;
  template<class K, class V, class Pr = less<K>, class Alloc = allocator<pair<const K, V>>> class multimap;
  
  template<class K, class V, class Hasher = hash<K>, class Eq = equal_to<K>,
           class Alloc = allocator<pair<const K, V>>> class unordered_map;
  
  template<class K, class V, class Hasher = hash<K>, class Eq = equal_to<K>,
           class Alloc = allocator<pair<const K, V>>> class unordered_multimap;
  
  template<class K, class V, class Tr = hash_compare<K, less<K>>, 
           class Alloc = allocator<pair<const K, V>>> class hash_map;
  
  template<class K, class V, class Tr = hash_compare<K, less<K>>,
           class Alloc = allocator<pair<const K, V>>> class hash_multimap;
  ```

- 成员函数

  ```cpp
  // 与 set 系列类似，新增以下函数
  
  // 若容器中已存在等效的键，则不执行任何操作。否则，行为类似于emplace。
  iterator try_emplace(const key_type& k, Args&&... args);
  pair<iterator, bool> try_emplace(const key_type& k, Args&&... args);
  
  // 如果容器中已经存在等效的键，则更新key的value，否则，插入新值，和insert一样
  iterator insert_or_assign(const_iterator hint, const key_type& k, M&& obj);
  pair<iterator, bool> insert_or_assign(const key_type& k, M&& obj);
  ```

#### 常用算法

- 算法复杂度

  | 算法复杂度 |   大O表示法   |       示例算法       |
  | :--------: | :-----------: | :------------------: |
  |    常数    |    $O(1)$     |     访问数组元素     |
  |    对数    | $O(\log{n})$  |       二分查找       |
  |    线性    |    $O(n)$     | 未排序列表中查找元素 |
  |  线性对数  | $O(n\log{n})$ |       归并排序       |
  |   二次方   |   $O(n^2)$    |       选择排序       |

- STL常用算法总结

|         算法          |                       常用版本                        |                             描述                             |                      返回                       |
| :-------------------: | :---------------------------------------------------: | :----------------------------------------------------------: | :---------------------------------------------: |
|      *std::find*      |               *find(first, last, val)*                |           在两个迭代器指定范围内查找第一个val元素            |         引用被查找值的迭代器或 *end()*          |
|    *std::find_if*     |           *find_if(first, last, callback)*            |       在两个迭代器指定范围内查找第一个与回调匹配的元素       |          引用匹配值的迭代器或 *end()*           |
|  *std::find_if_not*   |         *find_if_not(first, last, callback)*          |      在两个迭代器指定范围内查找第一个与回调不匹配的元素      |         引用不匹配值的迭代器或 *end()*          |
|     *std::count*      |               *count(first, last, val)*               |            在两个迭代器指定范围内统计val出现次数             |                    出现次数                     |
|    *std::count_if*    |           *count_if(first, last, callback)*           |            在两个迭代器指定范围内统计回调匹配次数            |                    匹配次数                     |
|    *std::generate*    |           *generate(first, last, callback)*           |      将指定范围内的值使用回调进行计算，回调输入必须为空      |                  计算得到的值                   |
|     *std::max()*      |            *max(left, right /\*, pred\*/)*            |                  通过 < 或指定谓词比较元素                   |              返回较大元素的常引用               |
|     *std::min()*      |            *min(left, right /\*, pred\*/)*            |                  通过 < 或指定谓词比较元素                   |              返回较小元素的常引用               |
|  *std::max_element*   |        *max_element(first, last /\*, pred\*/)*        |                   在指定范围内查找最大元素                   |           引用第一个最大元素的迭代器            |
|  *std::min_element*   |        *min_element(first, last /\*, pred\*/)*        |                   在指定范围内查找最小元素                   |           引用第一个最小元素的迭代器            |
| *std::adjacent_find*  |       *adjacent_find(first, last /\*, pred\*/)*       |               在指定范围内查找有连续重复的元素               |      第一个连续重复元素的迭代器或 *end()*       |
|     *std::all_of*     |              *all_of(first, last, pred)*              |             判断指定范围内元素是否全部与谓词匹配             |                     *bool*                      |
|     *std::any_of*     |              *any_of(first, last, pred)*              |             判断指定范围内是否存在元素与谓词匹配             |                     *bool*                      |
|    *std::none_of*     |             *none_of(first, last, pred)*              |            判断指定范围内元素是否全部与谓词不匹配            |                     *bool*                      |
|    *std::for_each*    |           *for_each(first, last, callback)*           |      对指定范围内元素执行一次回调，回调输入必须为值类型      |                    callback                     |
|   *std::transform*    |        *transform(first, last, dst, callback*)        |    对指定范围内元素执行回调，输出保存到从 dst 开始的位置     |           保存结束位置的下一个迭代器            |
|   *std::transform*    |    *transform(first, last, first1, dst, callback)*    | [first, last) 内的元素与 first1 开始的元素 zip，作为回调的两个输入参数，输出保存到 dst 开始的位置 |           保存结束位置的下一个迭代器            |
|     *std::equal*      |  *equal(first, last, first1, callback /\*, pred\*/)*  | [first, last) 内的元素与 first1 开始的元素一次调用 == 或回调进行比较，判断是否全部相等 |                      bool                       |
|      *std::copy*      |               *copy(first, last, dst)*                |         [first, last) 内的元素拷贝到 dst 开始的位置          |           拷贝结束位置的下一个迭代器            |
| *std::copy_backward*  |           *copy_backward(first, last, dst)*           | [first, last) 内的元素逆序拷贝到 dst 之前的位置，copy_backward 3 个参数都必须是可以自增或自减的双向迭代器 |              拷贝结束位置的迭代器               |
|    *std::copy_if*     |           *copy_if(first, last, dst, pred)*           | [first, last) 内的元素，若满足谓词，则拷贝到 dst 开始的位置  |           拷贝结束位置的下一个迭代器            |
|     *std::copy_n*     |                 *copy_n(src, n, dst)*                 |          从 src 开始拷贝 n 个元素到 dst 开始的位置           |           拷贝结束位置的下一个迭代器            |
| *std::partition_copy* |    *partition_copy(first, last, dst1, dst2, pred)*    | [first, last) 内的元素，若满足谓词，则拷贝到 dst1 开始的位置，否则拷贝到 dst2 开始的位置 | dst1、dst2拷贝结束位置的下一个迭代器组成的 pair |
|      *std::move*      |               *move(first, last, dst)*                |        [first, last) 内的元素，移动到 dst 开始的位置         |           拷贝结束位置的下一个迭代器            |
| *std::move_backward*  |           *move_backward(first, last, dst)*           |                  参考 copy_backward 和 move                  |              拷贝结束位置的迭代器               |
|    *std::replace*     |        *repalce(first, last, oldVal, newVal)*         |    [first, last) 内的元素，若值为 oldVal，则替换为 newVal    |                      void                       |
|   *std::replace_if*   |        *replace_if(first, last, pred, newVal)*        |     [first, last) 内的元素，若满足谓词，则替换为 newVal      |                      void                       |
|     *std::unique*     |          *unique(first, last /\*, pred\*/)*           | [first, last) 内的元素，若相邻元素相等或满足谓词，则视为重复，unique 后前面 n 个元素全部不重复，剩余元素保持与 unique之前相同，一般执行 unique 前需要事先排序 |          unique 区间之后的下一个迭代器          |
|  *std::unique_copy*   |     *unique_copy(first, last, dst /\*, pred\*/)*      | [first, last) 内的元素，若相邻元素相等或满足谓词，则视为重复，unique 将所有不重复的元素拷贝到 dst 开始的位置，一般执行 unique_copy 前需要事先排序 |         dst 拷贝结束位置的下一个迭代器          |
|    *std::reverse*     |                *reverse(first, last)*                 |                [first, last) 内的元素原地反转                |                      void                       |
|  *std::reverse_copy*  |           *reverse_copy(first, last, dst)*            |      [first, last) 内的元素反转，拷贝到 dst 开始的位置       |         dst 拷贝结束位置的下一个迭代器          |
|      *std::sort*      |           *sort(first, last /\*, pred\*/)*            |     [first, last) 内的元素，按照 < 或谓词进行比较和排序      |                      void                       |
|     *std::merge*      | *merge(first, last, first1, last1, dst /\*, pred\*/)* | 对有序的 [first, last) 和 [first1, last1) 两个区间进行归并，结果输出到 dst 开始的位置 |         dst 拷贝结束位置的下一个迭代器          |
|   *std::is_sorted*    |         *is_sorted(first, last /\*, pred\*/)*         |             判断 [first, last) 内的元素是否有序              |                      bool                       |
| *std::random_shuffle* |      *random_shuffle(first, last /\*, func\*/)*       | 使用 rand() 或自定义随机数生成器打乱 [first, last) 内的元素  |                      void                       |
|   *std::accumulate*   |      *accumulate(first, last, val /\*, func\*/)*      | 头文件\<numeric>，[first, last) 内的元素累加，累加的初值为 val；或 [first, last) 内的元素依次调用 func：tmp1 = func(val, first)，tmp2 = func(tmp1, first++)，... |               累加或函数执行结果                |
|      *std::iota*      |               *iota(first, last, val)*                |     [first, last) 内的元素，由 val 开始使用 ++ 递增赋值      |                      void                       |

- 集合算法

|              算法               |                           常用版本                           |                       描述                        |              返回              |
| :-----------------------------: | :----------------------------------------------------------: | :-----------------------------------------------: | :----------------------------: |
|         *std::includes*         |     *includes(first, last, first1, last1 /\*, pred\*/)*      |       验证第二个序列是否是第一个序列的子集        |              bool              |
|        *std::set_union*         |  *set_union(first, last, first1, last1, dst /\*, pred\*/)*   |    计算两个有序序列的并集，存入 dst 开始的位置    | dst 拷贝结束位置的下一个迭代器 |
|     *std::set_intersection*     | *set_intersection(first, last, first1, last1, dst /\*, pred\*/)* |    计算两个有序序列的交集，存入 dst 开始的位置    | dst 拷贝结束位置的下一个迭代器 |
|      *std::set_difference*      | *set_difference(first, last, first1, last1, dst /\*, pred\*/)* |    计算两个有序序列的差集，存入 dst 开始的位置    | dst 拷贝结束位置的下一个迭代器 |
| *std::set_symmetric_difference* | *set_symmetric_difference(first, last, first1, last1, dst /\*, pred\*/)* | 计算两个有序序列的对称集合差，存入 dst 开始的位置 | dst 拷贝结束位置的下一个迭代器 |

