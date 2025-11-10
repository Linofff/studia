#include <iostream>
using namespace std;

int main() {
  int x[] = {1, 2, 3, 4};
  int *p = &(x[3]);

  cout << (p - x - 2);
  return 0;
}
