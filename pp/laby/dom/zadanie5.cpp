#include <iostream>
using namespace std;
#define rozmiar 10

int main() {
  int tab[rozmiar] = {1, 2, 2, 3, 3, 5, 7, 7, 2, 1};
  for (int i = 0; i < rozmiar; i++) {
    cin >> tab[i];
  }
  char powtorzenie[rozmiar] = {};
  for (int i = 0; i < rozmiar; i++) {
    for (int j = i + 1; j < rozmiar; j++) {
      if (tab[i] == tab[j])
        powtorzenie[j] = 1;
    }
  }
  for (int i = 0; i < rozmiar; i++)
    if (!powtorzenie[i])
      cout << tab[i] << " ";
  return 0;
}
