#include <stdio.h>

void PrintBoard(char tab[][11]) {
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++)
      printf("%c", tab[i][j]);
    printf("\n");
  }
}

void Change(char tab[][11]) {
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      if ('*' != tab[i][j]) {
      } else {
        if ((tab[i - 1][j] - '0') <= 9 && (tab[i - 1][j] - '0') >= 0) {
          tab[i][j] = (tab[i - 1][j] - '0' + 1) + '0';
          for (int k = 1; k < 4; k++) {
            if (((tab[i - k][j] - '0') <= 9 && (tab[i - k][j] - '0') >= 0))
              tab[i - k][j] = tab[i][j];
            else
              break;
          }
        } else if ((tab[i][j - 1] - '0') <= 9 && (tab[i][j - 1] - '0') >= 0) {
          tab[i][j] = (tab[i][j - 1] - '0' + 1) + '0';
          for (int k = 1; k < 4; k++) {
            if (((tab[i][j - k] - '0') <= 9 && (tab[i][j - k] - '0') >= 0))
              tab[i][j - k] = tab[i][j];
            else
              break;
          }
        } else
          tab[i][j] = '1';
      }
    }
  }
}

int main() {
  char tab[10][11] = {"*..*......", "*..*....*.", "...*......", "*......**.",
                      "*..***....", "*......*.*", "*..*.....*", "...*.....*",
                      ".....*.*.*", "..*......."};

  PrintBoard(tab);
  printf("\n");
  Change(tab);
  PrintBoard(tab);

  return 0;
}
