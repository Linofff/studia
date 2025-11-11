#include <iostream>
using namespace std;
#define SIZE 6
int *f(int tab[], int z) {

  int *x = (int *)malloc(z * sizeof(int));
  int i, j = 0;
  for (i = 0; i < z; i++)

    if (tab[i] > 0)
      x[j++] = tab[i];

  while (j < z)

    x[j++] = 0;

  return x;
}
int main(int argc, char **argv) {
  int z[] = {-2, 3, 5, -1, 4, -3, 2, -4}, *w[2], d, i;

  w[0] = f(z, 2);
  d = w[0][0] + w[0][1];
  w[1] = f(z, SIZE);
  for (i = 0; i < SIZE; i++)
    d += w[1][i];
  cout << d;
  free(w[0]);
  free(w[1]);
  return 0;
}
