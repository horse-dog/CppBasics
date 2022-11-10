#include <iostream>
#include <sstream>
#include <tuple>

using namespace std;

template <class out_type, class in_value>
out_type convert(const in_value& t) {
  stringstream stream;
  stream << t;
  out_type result;
  stream >> result;
  return result;
}

tuple<double, double> parse_coord(const string& coord) {
  istringstream ism(coord);
  double x, y;
  ism >> x >> y;
  return {x, y};
}

template <class T>
string to_string(const T& t) {
  ostringstream oss;
  oss << t;
  return oss.str();
}

void test_stringstream() {
  auto d = convert<double>("12.56");
  auto s = convert<string>(9000.0);
  cout << "(double)\"12.56\": " << d << '\n';
  cout << "(string)9000: " << s << '\n';
}

void test_istringstream() {
  auto coord = parse_coord("3.14 1.23");
  cout << "x=" << get<0>(coord) << ", y=" << get<1>(coord) << '\n';
}

void test_ostringstream() {
  string s = to_string(123.456);
  cout << "string: " << s << '\n';
}

int main(int argc, char* argv[]) {
  test_stringstream();
  test_istringstream();
  test_ostringstream();
  return 0;
}