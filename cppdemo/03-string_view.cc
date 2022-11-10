/**
 * @file 3-string_view.cc
 * @author marjason (horsedog@whu.edu.com)
 * @brief
 * @version 0.1
 * @date 2022-07-29
 *
 * @copyright Copyright (c) 2022
 *
 *
 * @attention this file should be compiled with c++17,
 * for exmaple, command in g++ is:
 *  `g++ -std=c++17 .\3-string_view.cc`
 */

#include <iostream>
#include <string>
#include <string_view>

#include "utils/tick.h"

void FunctionWithString(const std::string& string) {}
void FunctionWithString(const std::string_view& stringView) {}

void test_string() {
  for (int i = 0; i < 1000000; i++) {
    std::string name = "Horse Dog";
    std::string firstName = name.substr(0, 5);
    std::string lastName = name.substr(6, -1);
    FunctionWithString(firstName);
    FunctionWithString(lastName);
  }
}

void test_string_view() {
  for (int i = 0; i < 1000000; i++) {
    const char* name = "Horse Dog";
    std::string_view firstName = std::string_view(name, 5);
    std::string_view lastName = std::string_view(name + 6, 3);
    FunctionWithString(firstName);
    FunctionWithString(lastName);
  }
}

int main() {
  TICK(test_string);
  TICK(test_string_view);
  return 0;
}