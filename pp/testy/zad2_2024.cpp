#include <iostream>
using namespace std;

int main(int argc, char **argv) {
  int i, x;
  char *s;
  for (x = i = 0; i < argc; i++) {
    for (s = argv[i]; *s != '\0'; s++)
      if (s - argv[i] > 1) {
        x++;
        break;
      }
  }
  cout << x;
  return 0;
}
