#include <iostream>

using namespace std;

int sum_1(int tab[], int size, int index) {
  if (index == size - 1) {
    cout << tab[index] << " ";
    return tab[index];
  } else {
    int temp = tab[index] + sum_1(tab, size, index + 1);
    cout << temp << " ";
    return temp;
  }
}

int sum_2(int tab[], int size) {
  if (size == 1) {
    cout << tab[size - 1] << " ";
    return tab[size - 1];
  } else {
    int temp = tab[size - 1] + sum_2(tab, size - 1);
    cout << temp << " ";
    return temp;
  }
}

int sum_3(int tab[], int forward, int backward) {
  if (forward == backward) { // for odd sizes
    cout << tab[forward] << " ";
    return tab[forward];
  } else if (forward > backward) { // for even sizes
    cout << 0 << " ";
    return 0;
  } else {
    int temp =
        tab[forward] + tab[backward] + sum_3(tab, forward + 1, backward - 1);
    cout << temp << " ";
    return temp;
  }
}

int sum_4(int tab[], int size) { // works in the same fashion as sum_1, but has
                                 // one parameter less
  if (size == 1) {
    cout << tab[0] << " ";
    return tab[0];
  } else {
    int temp = tab[0] + sum_4(tab + 1, size - 1);
    cout << temp << " ";
    return temp;
  }
}

int main() {
  int tab[] = {1, 2, 3, 4, 5, 6, 7, 8};
  cout << sum_1(tab, sizeof(tab) / sizeof(tab[0]), 0) << endl;
  cout << sum_2(tab, sizeof(tab) / sizeof(tab[0])) << endl;
  cout << sum_3(tab, 0, sizeof(tab) / sizeof(tab[0]) - 1) << endl;
  cout << sum_4(tab, sizeof(tab) / sizeof(tab[0])) << endl;
}
