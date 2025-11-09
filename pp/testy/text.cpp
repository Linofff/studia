#include <iostream>
using namespace std;

char *jump(char *s) { return s + 2; }
void back(char *s) {
  if (s[1] != '+') {
    back(jump(s + 1));
    cout << *s;
  }
}
int main() {
  char t[] = {"some-short-text+++++++"};
  back(t + 2);
  return 0;
}
