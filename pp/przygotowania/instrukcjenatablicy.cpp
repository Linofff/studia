#include <iostream>
using namespace std;
#define rozmiar 26
int main() {
  char x[rozmiar] = {};
  int powtorzenia[rozmiar] = {};
  char znak = '0';
  int dodanych = 0;

  while (znak != 'Q') {
    cin >> znak;

    if (znak == 'A') {
      char n;
      cin >> n;
      x[dodanych] = n;
      dodanych++;
      powtorzenia[(n - 'A')]++;

    } else if (znak == 'P') {
      int k;
      cin >> k;
      for (int i = 0; i < dodanych; i += k)
        cout << x[i] << " ";
      cout << endl;
    } else if (znak == 'M') {
      int maxindex = -1;
      int maximum = -1;
      for (int i = 0; i < rozmiar; i++) {
        if (powtorzenia[i] > maximum) {
          maximum = powtorzenia[i];
          maxindex = i;
        }
      }
      cout << char(maxindex + 'A') << " " << maximum << endl;
    } else if (znak == 'Q')
      break;
  }
  return 0;
}
