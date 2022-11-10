#pragma once
#include <chrono>
#include <iostream>

class Ticker {
 public:
  Ticker(const std::string tag)
      : _start(std::chrono::high_resolution_clock::now()), _tag(tag) {}

  ~Ticker() { std::cout << _tag << getTimerMilliSec() << " ms" << std::endl; }

  void update() { _start = std::chrono::high_resolution_clock::now(); };

  // get second
  double getTimerSecond() { return getTimerMicroSec() * 0.000001; };

  // get millisecond
  double getTimerMilliSec() { return getTimerMicroSec() * 0.001; };

  // get microsecond
  long long getTimerMicroSec() {
    return std::chrono::duration_cast<std::chrono::microseconds>(
               std::chrono::high_resolution_clock::now() - _start)
        .count();
  }

 private:
  std::string _tag;
  std::chrono::time_point<std::chrono::high_resolution_clock> _start;
};

template <class Fn, class... Args>
decltype(auto) tick(Fn &&fn, std::string tag, Args &&...args) {
  Ticker t("\033[32m[elapsed time] \033[1m\033[34m" + tag + "\033[0m\t: ");
  return fn(std::forward<Args>(args)...);
}

#define TICK(fn, ...) tick(fn, #fn, ##__VA_ARGS__)