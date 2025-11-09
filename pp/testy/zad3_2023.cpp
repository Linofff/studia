#include <iostream>
using namespace std;

int f(unsigned int x) {
  if (x == 1)
    return 1;
  else if (x % 2 == 1)
    return 0;
  else
    return f(x >> 1);
}
int main() {
  int t = 16;
  if (f(t) == 0) {
    cout << "nie";
  } else {
    cout << "tak";
  }
  return 0;
}
