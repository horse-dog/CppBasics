#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFSIZE 32

void test_strcpy() {
  const char *src = "hello!";
  char *dst = (char *)malloc(BUFFSIZE);
  strcpy(dst, src);
  printf("src: %s\n", src);
  printf("dst: %s\n", dst);
  free(dst);
}

void test_strncpy() {
  const char *src = "hello, world!";
  char *dst = (char *)malloc(BUFFSIZE);
  strncpy(dst, src, strlen("hello"));
  printf("src: %s\n", src);
  printf("dst: %s\n", dst);
  free(dst);
}

void test_strcat() {
  char *dst = (char *)malloc(BUFFSIZE);
  strcpy(dst, "hello");
  strcat(dst, ", world!");
  printf("dst: %s\n", dst);
  free(dst);
}

void test_strncat() {
  char *dst = (char *)malloc(BUFFSIZE);
  const char *src = ", world!";
  strcpy(dst, "hello");
  strncat(dst, src, strlen(src) - 1);
  printf("dst: %s\n", dst);
  free(dst);
}

void test_strlen() {
  const char *str = "hello!";
  printf("%ld\n", strlen(str));
}

void test_strcmp() {
  const char *str1 = "hello!";
  const char *str2 = "world!";
  auto flag = strcmp(str1, str2);
  switch (flag) {
    case -1:
      printf("str1 < str2\n");
      break;
    case 0:
      printf("str1 == str2\n");
      break;
    case 1:
      printf("str1 > str2\n");
      break;
    default:
      break;
  }
}

void test_strcasecmp() {
  const char *str1 = "hello!";
  const char *str2 = "HELLO!";
  auto flag = strcasecmp(str1, str2);
  switch (flag) {
    case -1:
      printf("str1 < str2\n");
      break;
    case 0:
      printf("str1 == str2\n");
      break;
    case 1:
      printf("str1 > str2\n");
      break;
    default:
      break;
  }
}

void test_strncmp() {
  const char *str1 = "hello!";
  const char *str2 = "hello, world!";
  auto flag = strncmp(str1, str2, strlen(str1) - 1);
  switch (flag) {
    case -1:
      printf("str1 < str2\n");
      break;
    case 0:
      printf("str1 == str2\n");
      break;
    case 1:
      printf("str1 > str2\n");
      break;
    default:
      break;
  }
}

void test_strchr() {
  const char *str = "hello, world!";
  auto result = strchr(str, 'w');
  printf("result: %s\n", result);
}

void test_strrchr() {
  const char *str = "hello, world!";
  auto result = strrchr(str, 'l');
  printf("result: %s\n", result);
}

void test_strstr() {
  const char *str = "hello, world!";
  auto result = strstr(str, "or");
  printf("result: %s\n", result);
}

int main(int argc, char *argv[]) {
  test_strcpy();
  test_strncpy();
  test_strcat();
  test_strncat();
  test_strlen();
  test_strcmp();
  test_strcasecmp();
  test_strncmp();
  test_strchr();
  test_strrchr();
  test_strstr();
  return 0;
}