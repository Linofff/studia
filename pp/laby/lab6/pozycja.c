#include <stdio.h>
#define SIZE 100

int Find(int tab[], int size, int szukana, int index) {
  if (*tab == szukana)
    return index;
  else if (index == size - 1)
    return -1;
  index++;
  return Find((tab + 1), size, szukana, index);
}

int main() {
  int tab[SIZE];
  int rozmiar;
  scanf("%d", &rozmiar);
  for (int i = 0; i < rozmiar; i++) {
    scanf("%d", &tab[i]);
  }
  int szukana;
  scanf("%d", &szukana);

  int index = Find(tab, rozmiar, szukana, 0);

  printf("%d", index);

  return 0;
}
