#include <cmath>
#include <iostream>
using namespace std;

void riddle(int setsize) {
  int subnum = (1 << setsize);
  for (int i = 0; i < subnum; i++) {
    cout << "{ ";
    for (int j = 1, el = 0; j < subnum; j *= 2, el++) {
      if (j & i)
        cout << el << " ";
    }
    cout << "}" << endl;
  }
}

int main() {
  int n = 4;
  riddle(n);
  return 0;
}
