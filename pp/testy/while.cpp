#include <climits>
#include <iostream>
using namespace std;
int main() {
  int i = 214748360;
  cin >> i;
  float x = i;
  int y = 0;
  while (x < (float)(i + 16)) {
    // cout << y << " " << x << endl;
    y++;
    x++;
  }
  return 0;
}
