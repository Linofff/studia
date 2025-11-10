#include <stdio.h>

int find(int tab[], int rozmiar, int szukana, int i) {
  if (*tab == szukana)
    return i;
  else if (i == rozmiar - 1)
    return -1;
  i++;
  return find((tab + 1), rozmiar, szukana, i);
}

int main() {
  int tab[10] = {1, 2, 3, 4, 50, 6, 7, 8, 9, 10};
  int rozmiar = (sizeof(tab) / sizeof(tab[0]));
  int szukana = 50;

  scanf("%d", &szukana);

  int index = find(tab, rozmiar, szukana, 0);
  printf("%d", index);

  return 0;
}
