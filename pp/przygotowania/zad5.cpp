#include <iostream>
using namespace std;

void f(char *p) {
  while (*(p) != 'a') {
    p++;
  }
  cout << p;
}
int main() {
  char str[] = {"SecondExam"};
  f(str);
  return 0;
}
