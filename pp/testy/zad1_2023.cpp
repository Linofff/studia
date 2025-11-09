#include <iostream>
using namespace std;
int f(int x) {
  x = 3;
  return x + 1;
}
int *g(int *x) {
  *x = 6;
  return x + 2;
}

void print(int tab[4]) {
  for (int i = 0; i < 4; i++)
    cout << tab[i] << " ";
  cout << endl;
}
int main() {
  int a[4];
  f(a[1]);
  print(a);
  f(f(a[3]));
  print(a);
  g(a);
  print(a);
  g(g(a + 1));
  print(a);
  return 0;
}
