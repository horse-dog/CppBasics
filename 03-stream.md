#### ***sstream***

- ***stringstream***
- ***istringstream***
- ***ostringstream***

#### ***fstream***

- ***ifstream***

  - 该数据类型表示输入文件流，用于从文件读取信息。

- ***ofstream***

  - 该数据类型表示输出文件流，用于创建文件并向文件写入信息。

- ***fstream***

  - 该数据类型通常表示文件流，且同时具有 *ofstream* 和 *ifstream* 两种功能，这意味着它可以创建文件，向文件写入信息，从文件读取信息。

- ***open*** 函数

  ```cpp
  ofstream outfile;
  outfile.open("file.dat", ios::out | ios::trunc);
  // equals with following
  ofstream outfile("file.dat", ios::out | ios::trunc);
  ```

- ***close*** 函数：关闭文件流

- 文件打开模式

  |  模式标志   |                             描述                             |
  | :---------: | :----------------------------------------------------------: |
  |  ios::app   |             追加模式。所有写入都追加到文件末尾。             |
  |  ios::ate   |                  文件打开后定位到文件末尾。                  |
  |   ios::in   |                      打开文件用于读取。                      |
  |  ios::out   |                      打开文件用于写入。                      |
  | ios::trunc  | 如果该文件已经存在，其内容将在打开文件之前被截断，即把文件长度设为 0。 |
  | ios::binary |                  以二进制方式对文件进行操作                  |

- 写入文件

  - 方式1：***operator <<***

  - 方式2：***ostream &put(char c)***

  - 方式3：***ostream &write(const char \*str, std::streamsize count)***

    ```cpp
    void file_write()
    {
    	ofstream outfile("test.txt"); // 以写的方式打开文件
    	outfile.put('c'); // 向文件写入一个字符'c'
    	outfile.write("hello world", 5); // 向文件写入长度为n的字符串
    	outfile << "hehe"; // 向文件写入字符串
    	outfile.close(); // 关闭文件
    }
    ```

- 读取文件

  - 方式1：***operator >>***

  - 方式2：***get*** 函数

  - 方式3：***read*** 函数

    ```cpp
    void file_read() {
      ifstream infile("test.txt");
      char c;
      string str;
      char *buf = new char[1024];
      infile.get(c);	// 'h'
    
      infile.seekg(0, ios::end);
      int length = infile.tellg();
      infile.seekg(0, ios::beg);
      infile.read(buf, length);	// "hello world"
      
      infile.seekg(0, ios::beg);
      infile >> str;	// "hello"
      delete[] buf;
    }
    ```

- ***tellg*** 与 ***tellp***

  - ***tellg***：返回读取偏移
  - ***tellp***：返回写入偏移

- ***seekg*** 与 ***seekp***

  - ***seekg***：设置读取偏移
  - ***seekp***：设置写入偏移

- ***getline***函数

  - ***istream &getline(istream &in, string &str)***：从输入流 *in* 读取一行到 *str*
  - ***istream &getline(istream &in, string &str, char elem)***：从输入流读取内容到 *str*，遇到 *elem* 终止读取

  