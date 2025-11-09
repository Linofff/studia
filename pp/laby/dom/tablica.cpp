#include <iostream>
using namespace std;
#define size 3
int main() {
  int tab1[size], tab2[size], join[2 * size];
  for (int i = 0; i < size; i++)
    cin >> tab1[i];
  for (int i = 0; i < size; i++)
    cin >> tab2[i];

  for (int i = 0; i < size * 2; i++) {
    if (i % 2 == 0)
      join[i] = tab1[i / 2];
    else
      join[i] = tab2[i / 2];
  }

  for (auto x : join)
    cout << x << " ";

  return 0;
}
