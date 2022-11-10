#include <fstream>
#include <iostream>
using namespace std;

void test_ifstream() {
  ifstream infile("../../cmakelists.txt");
  string str;
  infile >> str;
  cout << str << '\n';
  
  cout << infile.tellg() << endl;
  infile.seekg(0);

  getline(infile, str, '\n');
  cout << str << '\n';

  infile.close();
}

int main(int argc, char* argv[]) {
  test_ifstream();
  return 0;
}