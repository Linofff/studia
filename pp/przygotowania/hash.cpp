#include <iostream>
using namespace std;
typedef struct {
  int key;
  int hash;
} pairs_t;

int find(pairs_t *b, int n, int x) {
  while (--n >= 0)
    if ((b + n)->key == x)
      return (b + n)->hash;
  return -1;
}

int main() {
  cout << find(b, n, x);
  return 0;
}
