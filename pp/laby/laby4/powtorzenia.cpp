#include <iostream>
using namespace std;
int main() {
  int tab[100];
  int n;
  cin >> n;

  for (int i = 0; i < n; i++)
    cin >> tab[i];

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < i; j++) {
      if (tab[i] == tab[j]) {
        cout << tab[i] << " ";
        break;
      }
    }
  }

  return 0;
}
