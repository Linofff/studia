#include <iostream>
#include <ostream>
using namespace std;
int x;
int main() {
  int tab[3] = {0, 1, 2};
  int *x = tab;
  unsigned int u = 0;

  cout << sizeof(tab); /* I-1 */
  cout << endl;
  cout << sizeof(x) / sizeof(int); /* I-2 */
  cout << endl;
  cout << (27 ^ 11); /* I-3 */
  cout << endl;
  cout << *(x + 1) - 1; /* I-4 */
  cout << endl;
  cout << (*(++x) == tab[1] ? 1 : 0); /* I-5 */
  cout << endl;

  for (int i = 0; i < 3; i++)
    cout << tab[i] << " ";
  cout << endl;
  cout << (u + 1) - x[1]; /* I-6 */
  cout << endl;
  cout << &(tab[1]) - tab; /* I-7 */
  return 0;
}
