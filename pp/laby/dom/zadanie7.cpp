#include <iostream>
#include <utility>
using namespace std;

void sortowanie(float *tab, int rozmiar) {
  for (int i = 0; i < rozmiar; i++) {
    for (int j = 0; j < rozmiar; j++)
      if (tab[i] < tab[j])
        swap(tab[i], tab[j]);
  }
}

int main() {
  float tab[] = {1, 2, 3, 4, 5, 6, 7, 3, 2, 2, 3, 5, 6, 6, 7};
  int rozmiar = sizeof(tab) / sizeof(tab[0]);

  sortowanie(tab, rozmiar);

  for (auto x : tab)
    cout << x << " ";
  cout << endl;

  if (rozmiar % 2 == 1)
    cout << (tab[rozmiar / 2] + tab[(rozmiar / 2) + 1]) / 2;
  else
    cout << tab[rozmiar / 2];

  return 0;
}
