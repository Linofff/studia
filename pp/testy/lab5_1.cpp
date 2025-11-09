#include <iostream>
using namespace std;

char end(char *text) {
  static int i = -1;
  i++;
  return *(text + i);
}

int main() {
  int i = 0;
  char text[] = "ccccc";
  while (end(text))
    cout << ++i;
}
