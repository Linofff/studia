#include <stdio.h>
#define SIZE 10

void PrintBoard(char tab[SIZE][SIZE + 1]) {
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      printf("%c", tab[i][j]);
    }
    printf("\n");
  }
}

void Change(char tab[SIZE][SIZE + 1]) {
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      if (tab[i][j] != '*')
        continue;

      if (i > 0 && tab[i - 1][j] >= '0' && tab[i - 1][j] <= '9') {
        tab[i][j] = (((tab[i - 1][j] + 1) - '0') % 10) + '0';
        for (int k = 1; k < 10; k++) {
          if (i - k >= 0 && tab[i - k][j] >= '0' && tab[i - k][j] <= '9')
            tab[i - k][j] = tab[i][j];
          else
            break;
        }

      } else if (j > 0 && tab[i][j - 1] >= '0' && tab[i][j - 1] <= '9') {
        tab[i][j] = (((tab[i][j - 1] + 1) - '0') % 10) + '0';
        for (int k = 1; k < 10; k++) {
          if (j - k >= 0 && tab[i][j - k] >= '0' && tab[i][j - k] <= '9')
            tab[i][j - k] = tab[i][j];
          else
            break;
        }

      } else {
        tab[i][j] = '1';
      }
    }
  }
}

int main() {
  char tab[SIZE][SIZE + 1];
  for (int i = 0; i < SIZE; i++) {
    scanf("%s", &tab[i]);
  }

  Change(tab);
  PrintBoard(tab);

  return 0;
}
