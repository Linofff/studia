#include <iostream>

using namespace std;

// For a square of size 4x4 there are 10 lines to check, this is for one of them
int sum_first_row(int tab[][4]) {
  int sy = 0;
  int sx = 0;
  int sum = 0;
  for (int p = 0; p < 4; p++) // for each number in the first row...
  {
    int y = sy + 0 * p;
    int x = sx + 1 * p;
    sum += tab[y][x];
    // sum += tab[sy][sx++];
  }
  return sum;
} // there are 3 more rows and 6 other lines:(

int sum_any_row(int tab[][4], int sx, int sy) {

  int sum = 0;
  for (int p = 0; p < 4; p++) // for each number in a row...
  {
    int y = sy + 0 * p;
    int x = sx + 1 * p;
    sum += tab[y][x];
    // sum += tab[sy][sx++];
  }
  return sum;
} // we need to also sum columns and diagonals :(

int sum_any_line(int tab[][4], int sx, int sy, int dx, int dy) {

  int sum = 0;
  for (int p = 0; p < 4; p++) // for each number in a line...
  {
    int y = sy + dy * p;
    int x = sx + dx * p;
    sum += tab[y][x];
  }
  return sum;
}
bool isMagic(int tab[][4]) {
  // 4 rows + 4 columns + 2 diagonals = 10 lines in total
  // starting position
  int sx[] = {0, 0, 0, 0, 0, 1, 2, 3, 0, 3};
  int sy[] = {0, 1, 2, 3, 0, 0, 0, 0, 0, 0};
  // change in each iteration
  int dx[] = {1, 1, 1, 1, 0, 0, 0, 0, 1, -1};
  int dy[] = {0, 0, 0, 0, 1, 1, 1, 1, 1, 1};

  int sum, first;

  for (int i = 0; i < sizeof(dx) / sizeof(dx[0]); i++) // for each line...
  {
    sum = 0;
    sum_any_line(tab, sx[i], sy[i], dx[i], dy[i]);
    /*for (int p = 0; p < 4; p++)                //for each number in a line...
    {
            int y = sy[i] + dy[i] * p;
            int x = sx[i] + dx[i] * p;
            sum += tab[y][x];
    }*/
    if (i == 0)
      first = sum;
    else if (sum != first)
      return false;
  }
  return true;
}

int main() {
  int tab[4][4] = {
      {16, 3, 2, 13}, {5, 10, 11, 8}, {9, 6, 7, 12}, {4, 15, 14, 1}};
  // warmup before the solution
  cout << sum_first_row(tab) << endl;
  cout << sum_any_row(tab, 0, 1) << endl;         // sum the second row
  cout << sum_any_line(tab, 3, 0, -1, 1) << endl; // sum the ascending diagonal
                                                  // /
  // uses only 2 for loops
  cout << isMagic(tab);
  return 0;
}
