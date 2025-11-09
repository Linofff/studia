#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

#define n 50
#define x 100000

int main() {
  // srand(time(NULL));
  // // srand(11235813);
  // int x = 10000;
  // cin >> x;
  // double powtorka = 0;
  // int birthdays[n] = {};
  // int powtorki[365] = {};
  // for (int i = 0; i < x; i++) {
  //   for (int i = 0; i < n; i++) {
  //     birthdays[i] = rand() % 365;
  //     powtorki[birthdays[i]]++;
  //   }
  //   for (int i = 0; i < 365; i++)
  //     if (powtorki[i] > 1)
  //       powtorka += powtorki[i] - 1;
  // }
  //
  // cout << double(powtorka / n) << endl;

  int birthdays[n] = {};
  int rep = 0;
  for (int k = 0; k < x; k++) {
    for (int i = 0; i < n; i++)

      for (int i = 0; i < n; i++) {
        if (birthdays[i] == birthdays[j]) {
          rep++;
          break;
        }
      }
    if (rep)
      break;
  }
  if ()

    return 0;
}
