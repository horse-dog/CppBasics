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
  // 127.0.0.1 indicates the local network card on this machine
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
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