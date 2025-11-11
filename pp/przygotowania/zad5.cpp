#include <iostream>
using namespace std;
#define SIZE 6
int *f(int tab[], int z) {

  int *x = (int *)malloc(z * sizeof(int));
  int i, j = 0;
  for (i = 0; i < z; i++)

    if (tab[i] < 0)
      x[j++] = tab[i];

  while (j < z)
    x[j++] = 0;

  return x;
}

int main() {
  int w[] = {-2, 3, 5, -1, 4, -3, 2, -4}, *z[2], d, i;
  z[0] = f(w, 2);
  d = z[0][0] + z[0][1];
  z[1] = f(w, SIZE);
  for (i = 0; i < SIZE; i++)
    d += z[1][i];
  cout << -d;
  free(z[0]);
  free(z[1]);

  cout << endl << endl;

  int tab[] = {1, 2, 3};
  int *p = &(tab[2]);

  cout << &(tab[10]) - p;
  return 0;
}
