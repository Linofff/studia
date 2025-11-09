#include <iostream>
using namespace std;
struct a_t {
  char c;
  int i;
} s;
int tab[] = {3, 4, 5, 6};
int main() {
  int *p = &(tab[1]), *p1 = tab + 4;
  // cout << (13 ^ 7) << endl;
  // cout << (sizeof(s.c) + sizeof(s.i)) / (sizeof(s)) << endl;
  // cout << p1 << endl;
  // cout << p << endl;
  // cout << p1 - p << endl;

  // cout << 15 / 2 / ((float)2) << endl;
  cout << (sizeof tab) / (sizeof p);
  return 0;
}
