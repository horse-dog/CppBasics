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