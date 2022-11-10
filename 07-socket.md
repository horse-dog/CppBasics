#### 套接字状态转换

![](.\img\image-20220716020207328.png)

- 服务端状态变迁

  服务端创建套接字之后调用listen函数将套接字由一个未连接的主动套接字转换为被动套接字，指示内核应接受指向该套接字的连接请求，套接字状态由CLOSE转换为LISTEN，等待客户端连接。所以服务端是被动接收连接的，服务端会先收到SYN，收到之后会立马发送一个SYN+ACK（同一个报文），此时状态转换到SYN_RCVD并等待客户端回复ACK，此时套接字处于未完成连接队列中，如果收到ACK状态会转换到ESTABLISHED，套接字处于已完成连接队列中，已完成连接队列不能超过listen设置的最大连接个数。此后服务端和客户端可以进行数据交互，客户端接收完数据之后主动close套接字，此时服务端会收到FIN并回复ACK，状态转换到LOSE_WAIT，当服务端的应用层也close套接字时服务端会发出一个FIN到客户端，状态转换到LAST_ACK，然后会收到客户端回复的ACK，状态转换到CLOSED。

- 客户端状态变迁

  客户端创建套接字之后会connect服务器，这时客户端会发送一个SYN到服务器，状态转换到SYN_SENT并等待服务器的回复，收到服务端的回复SYN+ACK（同一个报文）之后客户端会回复ACK此时状态转换到ESTABLISHED，正常数据交互完成之后客户端会close套接字此时发送一个FIN报文，状态转换到FIN_WAIT_1，同时等待服务端的回复，此时有三种情况：

  - 收到服务端的ACK但此时服务端没有关闭套接字。状态转换到了FIN_WAIT_2，然后再等待服务端关闭套接字发出的FIN，如果收到则回复ACK，状态转换到TIME_WAIT状态，等待2MSL超时之后自动转换为CLOSED状态。
  - 服务端同时也在关闭套接字，此时客户端会收到FIN并发出ACK，状态转换到CLOSING，之后等待服务端回复ACK，若收到ACK则转到TIME_WAIT状态。
  - 服务器在收到客户端FIN之后立马关闭套接字，此时客户端会收到一个ACK和FIN并发出ACK，状态转换到TIME_WAIT状态。

- 同时打开

  同时打开时两端几乎在同时发送SYN，并进入SYN_SENT状态。当某一端收到SYN时，状态变为SYN_RCVD状态，收到SYN之后发出SYN+ACK，当双方都收到对端发送的SYN+ACK之后，套接字进入ESTABLISHED状态。同时打开链接需要交换4个报文比正常的3次握手多一个，而且双方既是客户端也是服务器，此种情况为双方同时connect对端。

- 同时关闭

  同时关闭时双方都是主动关闭，双方状态变迁一样。以某一方为例：发送一个FIN，进入FIN_WAIT_1状态，接收一个FIN同时发送一个ACK进入CLOSING状态，最后接收到对方回复的ACK进入TIME_WAIT状态。

#### 127.0.0.1、0.0.0.0、localhost

- 127.0.0.1：属于{127,}集合中的一个，而所有网络号为127的地址都被称之为回环地址，永远都不能出现在主机外部的网络中。
- 0.0.0.0：在服务器中，指本机上的所有IPV4地址。在路由中，0.0.0.0表示默认路由。
- localhost：localhost是一个域名，用于指代this computer或者this host，在大多数系统中，localhost被指向了IPV4的127.0.0.1和IPV6的::1。

#### TCP三次握手

- 客户端调用 *socket()* 函数创建套接字后，因为没有建立连接，所以套接字处于`CLOSED`状态；服务器端调用 *listen()* 函数后，套接字进入`LISTEN`状态，开始监听客户端请求。
- 当客户端调用 connect() 函数后，TCP协议会组建一个数据包，并设置 SYN 标志位，表示该数据包是用来建立同步连接的。同时生成一个随机数字 1000，填充“序号（Seq）”字段，表示该数据包的序号。完成这些工作，开始向服务器端发送数据包，客户端套接字就进入了`SYN-SEND`状态。
- 服务器端收到数据包，检测到已经设置了 SYN 标志位，就知道这是客户端发来的建立连接的“请求包”。服务器端也会组建一个数据包，并设置 SYN 和 ACK 标志位，SYN 表示该数据包用来建立连接，ACK 用来确认收到了刚才客户端发送的数据包。
  - 服务器生成一个随机数 2000，填充“序号（Seq）”字段。2000 和客户端数据包没有关系。
  - 服务器将客户端数据包序号（1000）加1，得到1001，并用这个数字填充确认号（Ack）字段。
  - 服务器为客户端生成一个新的套接字，并将这个套接字添加到未完成连接队列。
  - 服务器将数据包发出，服务器为客户端生成的套接字进入`SYN-RECV`状态。
- 客户端收到数据包，检测到已经设置了 SYN 和 ACK 标志位，就知道这是服务器发来的“确认包”。客户端会检测“确认号（Ack）”字段，看它的值是否为 1000+1，如果是就说明连接建立成功。
  - 接下来，客户端会继续组建数据包，并设置 ACK 标志位，表示客户端正确接收了服务器发来的“确认包”。同时，将刚才服务器发来的数据包序号（2000）加1，得到 2001，并用这个数字来填充“确认号（Ack）”字段。
  - 客户端将数据包发出，套接字进入`ESTABLISED`状态，表示连接已经成功建立。
- 服务器端收到数据包，检测到已经设置了 ACK 标志位，就知道这是客户端发来的“确认包”。
  - 服务器会检测“确认号（Ack）”字段，看它的值是否为 2000+1，如果是就说明连接建立成功，服务器为客户端生成的套接字进入`ESTABLISED`状态。
  - 服务器将客户端从未完成连接队列移动已建立连接队列。
- 至此，客户端和服务器都进入了`ESTABLISED`状态，连接建立成功，接下来就可以收发数据了。

#### 三次握手的目的

- 第一次：让服务器知道，从客户端到服务器的连接是通的。
- 第二次：让客户端知道，从服务器到客户端的连接是通的。
- 第三次：让服务器知道，从服务器到客户端的连接是通的。

#### TCP四次挥手

- 第一次挥手：主机1（可以是客户端，也可以是服务器端），设置 *Seq* 和 *Ack*，向主机2发送一个 *FIN* 报文段（设置 FIN 标志位）；此时，主机1进入 **FIN_WAIT_1** 状态；这表示主机1没有数据要发送给主机2了。
- 第二次挥手：主机2收到了主机1发送的 *FIN* 报文段，向主机1回一个 *ACK* 报文段（设置 ACK 标志位），*Ack* 加1；主机2进入**CLOSE_WAIT** 状态。此时的TCP处于**半关闭状态**，主机1到主机2的连接释放。主机1收到来自主机2的 ACK 报文段后，进入 **FIN_WAIT_2** 状态。
- 第三次挥手：主机2也打算断开连接，向主机1发送连接释放报文段（设置 FIN 标志位），之后主机2进入 **LAST_ACK** 状态，等待主机1的确认。
- 第四次挥手：主机1收到来自主机2的连接释放报文段后，会向服务器发送一个 ACK 应答报文段（设置 ACK 标志位）。之后主机1进入 **TIME_WAIT** 状态，主机2收到 ACK 应答报文段后，服务器就进入 **CLOSED** 状态，到此主机2到主机1的连接释放。主机1处于 **TIME_WAIT** 状态时，TCP连接还未释放掉，需要等待 2MSL 后，客户端才进入 **CLOSED** 状态。

#### 等待2MSL的意义：以客户端主动关闭为例

- 保证客户端最后发送的ACK能够到达服务器，帮助其正常关闭。
  - 由于这个ACK报文段可能会丢失，使得处于LAST_ACK状态的服务器得不到对已发送FIN报文段的确认，从而会触发超时重传。服务器会重发FIN报文段，客户端能保证在2MSL时间内收到来自服务器的重传FIN报文段，从而客户端重新发送ACK应答报文段，并重置2MSL计数。
  - 但未达到2MSL时，若客户端重新发起SYN报文段请求和服务器端建立新的连接时，服务端会发送RST报文段给客户端，连接建立的过程就会被终止。
- 防止已失效的连接请求报文段出现在本连接中。

#### socket：创建套接字

```cpp
int socket(int af, int type, int protocol);
```

- `af`：地址族 *Address Family*，常用的有 **AF_INET** 和 **AF_INET6**，即 **IPv4** 和 **IPv6**。也可以使用 PF 前缀，PF 是 *Protocol Family* 的简写，它和 AF 是一样的。例如，**PF_INET** 等价于 **AF_INET**，**PF_INET6** 等价于 **AF_INET6**。
- `type`：数据传输方式，常用的有 **SOCK_STREAM** (面向连接) 和 **SOCK_DGRAM** (无连接)
- `protocol`：传输协议，常用的有 **IPPROTO_TCP** 和 **IPPTOTO_UDP**，分别表示 **TCP** 传输协议和 **UDP** 传输协议。
- 一般情况下有了 `af` 和 `type` 两个参数就可以创建套接字了，操作系统会自动推演出协议类型。
- 如果有两种不同的协议支持同一种地址类型和数据传输类型。当未指明使用哪种协议时，操作系统是无法自动推演。
- 成功时返回描述符，错误返回 -1，并设置 *errno*。

#### bind：为套接字绑定 IP地址和端口

```cpp
int bind(int sock, struct sockaddr *addr, socklen_t addrlen);

struct sockaddr_in {              // 存储 IPv4 地址与端口号的结构体
    sa_family_t     sin_family;   // 地址族（Address Family），也就是地址类型
    uint16_t        sin_port;     // 16位的端口号
    struct in_addr  sin_addr;     // 32位IP地址
    char            sin_zero[8];  // 不使用，一般用0填充
};

struct sockaddr_in6 {             // 存储 IPv6 地址与端口号的结构体
    sa_family_t sin6_family;      // 地址类型，取值为AF_INET6
    in_port_t sin6_port;          // 16位端口号
    uint32_t sin6_flowinfo;       // IPv6流信息
    struct in6_addr sin6_addr;    // 具体的IPv6地址
    uint32_t sin6_scope_id;       // 接口范围ID
};

struct sockaddr {                 // 通用的用于存储 IP 地址与端口号的结构体
    sa_family_t  sin_family;      // 地址族（Address Family），也就是地址类型
    char         sa_data[14];     // IP地址和端口号
};

struct in_addr {				// 存储 IPv4 地址的结构体
    in_addr_t  s_addr;            // 32位的IP地址
};

struct in6_addr {				// 存储 IPv6 地址的结构体
  	union {
    	uint8_t  __u6_addr8[16];
    	uint16_t __u6_addr16[8];
    	uint32_t __u6_addr32[4];
  	} __in6_u;
};
```

- `sock`：*socket* 文件描述符。
- `addr`：*sockaddr* 结构体变量的指针，调用 *bind* 函数前需要预先指定好 *addr* 指向的结构体内的各成员变量。
- `addrlen`：*addr* 变量的大小，可由 *sizeof()* 计算得出。
- 成功时返回0，错误时返回 -1，并设置 *errno*。

#### connect：将套接字连接到指定 IP 地址的指定端口

```cpp
int connect(int sock, struct sockaddr *serv_addr, socklen_t addrlen);
```

- `sock`：*socket* 文件描述符。
- `serv_addr`：存储了服务器的 IP地址和端口号的结构体的指针
- `addrlen`：*addr* 变量的大小，可由 *sizeof()* 计算得出。
- 成功时返回0，错误时返回 -1，并设置 *errno*。

#### listen：使套接字进入被动监听状态

```cpp
int listen(int sock, int backlog);
```

- `sock`：*socket* 文件描述符。
- `backlog`：决定 *accept* 队列的最大规模。
- 成功时返回0，错误时返回 -1，并设置 *errno*。

#### accept：当套接字处于监听状态时，可以通过 accept() 函数来接收客户端请求

```cpp
int accept(int sock, struct sockaddr *addr, socklen_t *addrlen);
```

- `sock`：*socket* 文件描述符。
- `addr`：*sockaddr* 结构体变量的指针，用于存储接受的客户端的 IP 地址和端口号。
- `addrlen`：*socklen_t* 类型的变量的指针，存储 *addr* 变量的大小，可由 *sizeof()* 计算得出，*accept* 函数会重新设置 *addrlen* 为实际接收的 *sockaddr* 的大小。
- 若 *sock* 为阻塞的，则 *accept()* 会阻塞程序执行，直到有新的请求到来。
- 若 *sock* 为非阻塞的，则 *accept()* 不会阻塞，若没有新的请求到来，*accept()* 函数直接返回 -1，并设置 *errno* 为 **EAGAIN**，非阻塞的系统调用，由于资源限制或者不满足条件，导致返回值为 **EAGAIN**。
-  accept()函数不参与三次握手，而只负责从已建立连接队列中取出一个连接和 *sock* 进行绑定。
- *accept* 函数返回一个新的套接字来和客户端通信，错误时返回 -1。

#### htons、htonl、ntohs、ntohl

- htons：2字节数据主机序转网络序。
- htonl：4字节数据主机序转网络序。
- ntohs：2字节数据网络序转主机序。
- ntohl：4字节数据网络序转主机序。

#### inet_addr

- 将字符串表示的IPv4地址转换为4字节网络序表示。

#### TCP 数据发送

```cpp
ssize_t write(int sockfd, const void *buf, size_t nbytes);
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
```

- *write* 函数使用与文件IO一致。
- *send* 函数中 *flags* 参数为调用的执行方式（阻塞/非阻塞），当 *flags* 设置为0时，可以使用函数 *write()* 来代替 *send()* 函数。
- 两函数成功时返回实际发送的字节数，失败时返回-1，并设置 *errno*。

#### TCP 数据接收

```cpp
ssize_t read(int sockfd, void *buf, size_t nbytes);
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
```

- read 函数使用与文件IO一致。
- recv 函数中 *flags* 参数为调用的执行方式（阻塞/非阻塞），当 *flags* 设置为0时，可以使用函数 *read()* 来代替 *recv()* 函数。
- 两函数成功时返回实际接收的字节数，失败时返回-1，并设置 *errno*。

#### UDP 数据发送

```cpp
ssize_t sendto(int sock, void *buf, size_t nbytes, int flags, struct sockaddr *to, socklen_t addrlen);
```

- sendto 函数类似与 TCP send 函数，但需要额外指定对端地址。

#### UDP 数据接收

```cpp
ssize_t recvfrom(int sock, void *buf, size_t nbytes, int flags, struct sockadr *from, socklen_t *addrlen);
```

- recvfrom 函数类似与 TCP recv 函数，但需要额外指定对端地址。

#### TCP 示例

- server

```cpp
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 1024
int main() {
  int server_sock = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in server_addr, client_addr;
  socklen_t nSize = sizeof(client_addr);

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = PF_INET;
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_addr.sin_port = htons(1234);

  bind(server_sock, (sockaddr*)&server_addr, sizeof(server_addr));
  listen(server_sock, 20);

  while (1) {
    char buffer[BUF_SIZE] = {0};
    int client_sock = accept(server_sock, (sockaddr*)&client_addr, &nSize);
    int n = recv(client_sock, buffer, BUF_SIZE, 0);
    send(client_sock, buffer, n, 0);
    close(client_sock);
  }

  close(server_sock);
  return 0;
}
```

- client

```cpp
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 1024
int main() {
  sockaddr_in server_addr;

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = PF_INET;
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");	// 127.0.0.1 表示本机上的本网卡
  server_addr.sin_port = htons(1234);

  while (1) {
    char buffer[BUF_SIZE] = {0};
    printf("Input a string: ");
    fgets(buffer, BUF_SIZE, stdin);
    if (strcmp(buffer, "quit\n") == 0) break;

    int client_sock = socket(PF_INET, SOCK_STREAM, 0);
    connect(client_sock, (sockaddr*)&server_addr, sizeof(server_addr));
    
    send(client_sock, buffer, strlen(buffer), 0);
    int n = recv(client_sock, buffer, BUF_SIZE, 0);
    buffer[n] = 0;
    printf("Message form server: %s", buffer);
    close(client_sock);
  }

  return 0;
}
```

#### UDP 示例

- server

```cpp
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 1024
int main() {
  int server_sock = socket(AF_INET, SOCK_DGRAM, 0);

  sockaddr_in server_addr, client_addr;
  socklen_t nSize = sizeof(client_addr);

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = PF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);	// INADDR_ANY 表示的是一个本机上所有的网卡
  server_addr.sin_port = htons(1234);

  bind(server_sock, (sockaddr*)&server_addr, sizeof(server_addr));

  while (1) {
  	char buffer[BUF_SIZE];
    int n = recvfrom(server_sock, buffer, BUF_SIZE, 0, (sockaddr*)&client_addr, &nSize);
    sendto(server_sock, buffer, n, 0, (sockaddr*)&client_addr, nSize);
  }
  close(server_sock);
  return 0;
}
```

- client

```cpp
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 1024
int main() {
  int client_sock = socket(PF_INET, SOCK_DGRAM, 0);

  sockaddr_in server_addr;

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = PF_INET;
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_addr.sin_port = htons(1234);

  while (1) {
    char buffer[BUF_SIZE] = {0};
    printf("Input a string: ");
    fgets(buffer, BUF_SIZE, stdin);
    if (strcmp(buffer, "quit\n") == 0) break;
    sendto(client_sock, buffer, strlen(buffer), 0, (sockaddr*)&server_addr, sizeof(server_addr));
    int n = recvfrom(client_sock, buffer, BUF_SIZE, 0, NULL, NULL);
    buffer[n] = 0;
    printf("Message form server: %s", buffer);
  }
  return 0;
}
```
