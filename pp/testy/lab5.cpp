// 1)
// #include <iostream>
// using namespace std;
//
// char end(char *text) {
//   static int i = -1;
//   ++i;
//   return *(text + i);
// }
//
// int main() {
//   int i = 0;
//   char text[] = "012345678";
//   while (end(text))
//     cout << ++i;
// }
// 2)

#include <iostream>
using namespace std;

int x(int p) { return p *= 2; }
int y(int *p) { return *p = *p * 2; }
int z(int *p) {
  *p += 2;
  return *p;
}

int main() {
  int t[] = {1, 10, 100, 1000};
  // int t[] = {2, 10, 100, 1000};
  int p = 1;
  cout << x(p) + x(p) << endl;   // ...
  cout << y(&p) + y(&p) << endl; // ...
  cout << y(t) + y(&p) << endl;  // ...
  cout << z(t) + z(t) << endl;   // ...
  return 0;
}
