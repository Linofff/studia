#include <stdio.h>

int main() {
  int n = 0;
  scanf("%d", &n);

  // Top part
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < i; j++)
      printf("*");
    printf("\\");
    for (int j = 0; j < (4 * n) - 2 - (2 * i); j++)
      printf("*");
    printf("/");
    for (int j = 0; j < i; j++)
      printf("*");
    printf("\n");
  }

  // Middle section
  for (int i = 0; i < (2 * n); i++) {
    for (int j = 0; j < n; j++)
      printf("*");

    if (i == 0 || i == (n * 2) - 1) {
      for (int j = 0; j < 2 * n; j++)
        printf("#");
    } else {
      printf("|");
      for (int j = 0; j < (4 * n) - 2 - (n * 2); j++)
        printf("?");
      printf("|");
    }

    for (int j = 0; j < n; j++)
      printf("*");

    printf("\n");
  }

  // Bottom part
  for (int i = n; i < n * 2; i++) {
    for (int j = 0; j < i; j++)
      printf("*");
    printf("\\");
    for (int j = 0; j < (4 * n) - 2 - (2 * i); j++)
      printf("*");
    printf("/");
    for (int j = 0; j < i; j++)
      printf("*");
    printf("\n");
  }

  return 0;
}
