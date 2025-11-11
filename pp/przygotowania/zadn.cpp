#include <iostream>
using namespace std;

int check(const char *s) { return (s[1] != '\0'); }

int main(int argc, char **argv) {
  while (--argc >= 0)
    if (check(argv[argc]))
      cout << argv[argc][0];
  return 0;
}
