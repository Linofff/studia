#include <iostream>
using namespace std;

const int iloscgraczy = 4;

int main() {
  int iloscpol;
  cin >> iloscpol;

  int gracze[iloscgraczy] = {};
  short poprzednia[iloscgraczy] = {};
  unsigned char z = 0;
  bool endofgame = false;
  char znak;
  int liczba;
  int gracz = 0;
  int winner = -1;

  while (!endofgame) {
    gracz %= iloscgraczy;
    cin >> znak;

    if (znak == 'M') {
      poprzednia[gracz] = liczba;

      cin >> liczba;

      int sBit = 7 - 2 * gracz;
      int wBit = 6 - 2 * gracz;

      if (((z & (1 << sBit)) == 0) && liczba == 6 && poprzednia[gracz] == 1) {
        z |= (1 << sBit);
        gracz++;
        continue;
      }

      if (z & (1 << sBit)) {
        gracze[gracz] += liczba;

        for (int i = 0; i < iloscgraczy; i++) {
          if (i != gracz && (z & (1 << (7 - 2 * i))) &&
              gracze[gracz] == gracze[i]) {
            cout << "Player " << i + 1 << " sent back to start!" << endl;
            gracze[i] = 0;
            z &= ~(1 << (7 - 2 * i));
            z &= ~(1 << (6 - 2 * i));
          }
        }
      }

      if (gracze[gracz] >= iloscpol) {
        endofgame = true;
        winner = gracz;
        break;
      }

      if (gracze[gracz] % 2 == 1)
        z |= (1 << wBit);
      else
        z &= ~(1 << wBit);

      gracz++;
    } else if (znak == 'P') {
      for (int i = 0; i < iloscgraczy; i++)
        cout << gracze[i] << " ";
      cout << "| Bits: ";
      for (int i = 7; i >= 0; i--)
        cout << ((z >> i) & 1);
      cout << endl;
    } else {
      return 0;
    }
  }

  cout << "Wygrał gracz nr: " << winner + 1 << endl;
  return 0;
}
