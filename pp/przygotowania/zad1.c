#include <stdio.h>

int find(int tab[], int rozmiar, int szukana) {
  static int i = 0;
  if (*tab == szukana)
    return i;
  else if (i == rozmiar - 1)
    return -1;
  i++;
  return find((tab + 1), rozmiar, szukana);
}

int main() {
  int tab[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int rozmiar = (sizeof(tab) / sizeof(tab[0]));
  int szukana = 7;

  scanf("%d", &szukana);

  int index = find(tab, rozmiar, szukana);
  printf("%d", index);

  return 0;
}
