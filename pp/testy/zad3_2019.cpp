#include <iostream>
using namespace std;
#define SIZE 12
int main() {
  char s[SIZE], str[] = {"programming"};
  int i = 0, j = 5;
  while (str[i] != '\0') {
    s[j--] = str[i++];
    if (j < 0)
      j = SIZE - 1;
  }
  s[j] = str[i];
  cout << s;
  return 0;
}
