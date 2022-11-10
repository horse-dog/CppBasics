#pragma once
#include <boost/type_index.hpp>
#include <string>

template <typename T>
std::string type_name(T val) {
  using boost::typeindex::type_id_with_cvr;
  return type_id_with_cvr<T>().pretty_name();
}