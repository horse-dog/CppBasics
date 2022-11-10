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
  // INADDR_ANY indicates all network cards on this machine
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
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