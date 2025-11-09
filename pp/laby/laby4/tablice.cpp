#include <iostream>
using namespace std;
int main() {
  char znaki[26];
  // int powtorzenia[52] = {};

  char znak = '0';
  int pozycja = 0;

  while (znak != 'Q') {
    cin >> znak;
    int pow = 0;
    if (znak == 'A') {
      char n;
      cin >> n;
      znaki[pozycja % 26] = n;
      pozycja++;
      // if (n >= 'A' && n <= 'Z')
      //   powtorzenia[n - 'A' + 26]++;
      // else if (n >= 'a' && n <= 'z')
      //   powtorzenia[n - 'a']++;

    } else if (znak == 'P') {
      int k;
      cin >> k;
      for (int i = 0; i < pozycja && i < 26; i += k)
        cout << znaki[i] << " ";
      cout << endl;
    } else if (znak == 'M') {
      // int maxindex = -1;
      // int maximum = -1;
      // for (int i = 0; i < 52; i++) {
      //   if (powtorzenia[i] > maximum) {
      //     maximum = powtorzenia[i];
      //     maxindex = i;
      //   }
      // }
      // if (maxindex < 26)
      //   cout << maximum << endl;
      // else
      //   cout << maximum << endl;
      for (int i = 0; i < pozycja && i < 26; i++) {
        int temp = 0;
        for (int j = 0; j < pozycja && j < 26; j++) {
          if (znaki[i] == znaki[j])
            temp++;
        }
        if (temp > pow)
          pow = temp;
      }
      cout << pow << endl;
    } else if (znak == 'Q')
      break;
  }
  return 0;
}
