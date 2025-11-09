#include <iostream>
using namespace std;
#define rozmiar 10
int main() {
  int x[rozmiar] = {1, 2, 3, 1, 2, 1, 6, 2, 6, 10};
  for (int i = 0; i < rozmiar; i++)
    cin >> x[i];

  for (int i = 0; i < rozmiar; i++) {
    for (int j = 0; j < i; j++) {
      if (x[i] == x[j]) {
        cout << x[i] << " ";
        continue;
      }
    }
  }
  return 0;
}
