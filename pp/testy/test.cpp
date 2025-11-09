#include <iostream>
using namespace std;
int main() {

  int x[] = {1, 4, 7, 9, 17};
  int *p = &(x[2]);

  int a = 5;

  // cout << (a << 2) + (a--); /* I-1 */
  // cout << (--a) + (--a) << endl;
  cout << x;

  return 0;
}
