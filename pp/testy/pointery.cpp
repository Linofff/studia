#include <iostream>
using namespace std;
int main() {
  int x = 5;
  int *pi = &x;
  int **ppi = &pi;
  int y = x;
  cout << x << " " << pi << " " << ppi << " " << y << endl;
  cout << x << " " << *pi << " " << *ppi << " " << y << endl;
  cout << x << " " << *pi << " " << **ppi << " " << y << endl;
  *ppi = &y;
  *pi = 6;
  // cout << x << " " << y << endl;
  **ppi = 7;
  cout << x << " " << y << endl;
  return 0;
}
