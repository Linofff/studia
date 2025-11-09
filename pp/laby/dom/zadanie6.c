#include <stdio.h>

#define ROZMIAR 10

int main() {
  int n, i, j;

  printf("Podaj liczbe elementow: ");
  scanf("%d", &n);

  int tab[n];
  int ranking[n];

  printf("Podaj %d liczb:\n", n);
  for (i = 0; i < n; i++) {
    scanf("%d", &tab[i]);
  }

  for (i = 0; i < n; i++) {
    int wieksze = 0;
    for (j = 0; j < n; j++) {
      if (tab[j] > tab[i])
        wieksze++;
    }
    ranking[i] = wieksze + 1;
  }

  printf("\nPozycje w rankingu:\n");
  for (i = 0; i < n; i++) {
    printf("%d ", ranking[i]);
  }

  return 0;
}
