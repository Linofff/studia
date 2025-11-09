#include <iostream>
using namespace std;

int x(int p) { return p += 1; }
int y(int *p) { return *p = *p + 1; }
int z(int *p) {
  p += 1;
  return *p;
}

int main() {
  int t[] = {1, 10, 100};
  // int t[] = {3, 10, 100};
  int p = 1010;
  cout << x(p) + x(p) << endl;
  cout << y(&p) + y(&p) << endl;
  cout << y(t) + y(t) << endl;
  cout << z(t) + z(t) + z(t) << endl;
  return 0;
}
