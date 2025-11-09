#include <iostream>
#include <iterator>
#include <stdlib.h>
#include <time.h>
#define SIZE 10
#define EMPTY 0
using namespace std;

/*
1. Complete the functions such that:
empty spots are denoted as 0;
ships are denoted by their size and do not collide (adjacent spots are allowed).
*/

void fill_board(char board[][SIZE]) {
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      board[i][j] = EMPTY;
    }
  }
}

#define VERTICAL 1
#define HORIZONTAL 0

// check if the ship can be placed on the position - if not, return false
bool check_position(char board[][SIZE], int ship_size, int direction, int x,
                    int y) {
  while (ship_size > 0) {
    if (board[y][x] != EMPTY) {
      return false;
      if (direction == VERTICAL)
        y++;
      if (direction == HORIZONTAL)
        x++;
    }
    ship_size--;
  }
  return true;
}

void place_ship(char board[][SIZE], int ship_size, int direction, int x,
                int y) {
  int i = ship_size;
  while (i > 0) {
    board[y][x] = ship_size;
    if (direction == VERTICAL)
      y++;
    if (direction == HORIZONTAL)
      x++;
    i--;
  }
}

/*
   ABCDE
0 |**333
1 |*1***
2 |*****
3 |**2**
4 |**2**

*/

void print_board(char board[][SIZE]) {
  cout << "  ";
  for (int i = 0; i < SIZE; i++) {
    cout << char('A' + i);
  }
  cout << endl;

  for (int i = 0; i < SIZE; i++) {
    cout << i << "|";
    for (int j = 0; j < SIZE; j++) {
      if (board[i][j] == EMPTY)
        cout << '*';
      else
        cout << '!';
    }
    cout << endl;
  }
}

void set_board(char board[][SIZE]) {
  // Fill the board with empty spaces
  fill_board(board);

  /*We need to RANDOMLY place our ships on the board:
  4 ships of size 1
  3 ships of size 2
  2 ships of size 3
  1 ship of size 4
  */
  for (int ship_size = 1; ship_size <= 4; ship_size++) {
    int ship_count = 5 - ship_size;
    for (int j = 1; j <= ship_count; j++) {
      int direction, x, y;
      do {
        // get a random number for a direction of the ship
        // 0 - horizontal, 1 - vertical
        direction = rand() % 2;
        // choose randomly a position of the ship
        x = rand() % SIZE;
        y = rand() % SIZE;
      } while (!check_position(board, ship_size, direction, x, y));
      // now we just need to place our ship on the board
      place_ship(board, ship_size, direction, x, y);
    }
  }
}

void shoot_command(char board[SIZE][SIZE]) {
  char x;
  int y;
  cout << "Coordinates: x (letter) y (number)";
  cin >> x >> y;
  /*
     2. In the condition add a function that handles the mechanics of shooting
     3. change the program in such a way that
                             initially all ships are invisible
                             (e.g. marked by negative numbers)
                             only parts of ships that were hit are visible
   */
  if (0 == 1) {
    cout << "Hit!" << endl;
  } else {
    cout << "Miss!" << endl;
  }
}

void play(char board[][SIZE]) {
  char option;
  bool exit = false;
  while (!exit) {
    cout << "Choose an option:" << endl;
    cout << "S - shoot" << endl;
    cout << "C - cheat" << endl;
    cout << "Q - end" << endl;
    cin >> option;
    switch (option) {
    case 'S':
      shoot_command(board);

      break;
    case 'C':
      /*
      4. Cheating enabled: this option shows the position of all ships once
      in the same fashion as before the shooting change
      */
      break;
    case 'Q':
      exit = true;
      break;
    default:
      cout << "Invalid option!" << endl;
    }
    print_board(board);
  }
}

int main(int argc, char *argv[]) {
  srand(time(NULL));
  // Declaration of the game board
  char board[SIZE][SIZE];
  set_board(board);
  print_board(board);
  play(board);
}
