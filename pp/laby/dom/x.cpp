
#include <iostream>
#define SIZE 15
using namespace std;

int main() {
  char tab[SIZE][SIZE + 1];
  int i, j;

  for (i = 0; i < SIZE; i++) {
    for (j = 0; j < SIZE; j++)
      tab[i][j] = (i == j || i + j == SIZE - 1) ? '*' : ' ';
    tab[i][j] = '\0'; // zakończenie linii znakiem null
  }

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      if ((i == 0) || (j == 0) || (i == SIZE - 1) || (j = SIZE - 1)) {
        tab[i][j] = '#';
      }
    }
  }
  for (i = 0; i < SIZE; i++)
    cout << tab[i] << endl;

  return 0;
}
