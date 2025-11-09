#include <cmath>
#include <iostream>
using namespace std;

/*
 *   print all subsets of a set of size setsize
 *   there are 2 ^ setsize subsets
 */
int riddle(unsigned int setsize) {
  int count = 0;
  unsigned int sub_num = (1 << setsize);
  for (unsigned int i = 0; i < sub_num; i++) {
    cout << "{ ";
    for (unsigned int j = 1, el = 0; j < sub_num; j *= 2, el++)
      if (j & i)
        cout << el << " ";
    cout << " }" << endl;

    count++;
  }
  return count;
}

// function returns the number of '1's in a binary representation
unsigned int ones(unsigned int num) {
  unsigned int count = 0;
  while (num) {
    if (num % 2 == 1)
      count++;
    num /= 2;
  }
  return count;
}
/*
 *   print all subsets of a set of size setsize with elnum elements
 *   combinatorics question: how many are there?
 */
int riddle2(unsigned int setsize, unsigned int elnum) {
  int count = 0;
  unsigned int sub_num = (1 << setsize);
  for (unsigned int i = 0; i < sub_num; i++) {
    if (elnum == ones(i)) { // omit executions for "wrong" numbers of elements
      cout << "{ ";
      for (unsigned int j = 1, el = 0; j < sub_num; j *= 2, el++)
        if (j & i) {

          cout << el << " ";
        }
      cout << "}" << endl;
      count++;
    }
  }
  return count;
}

/*
 * function returns the next number with the same number of '1's in a binary
 * representation
 * https://graphics.stanford.edu/~seander/bithacks.html#NextBitPermutation
 * Compute the lexicographically next bit permutation you can find alternative
 * solutions
 */
unsigned int next(unsigned int x) {
  unsigned int v = x; // current permutation of bits
  unsigned int w;     // next permutation of bits

  unsigned int t = (v | (v - 1)) + 1;
  w = t | ((((t & -(signed)t) / (v & -(signed)v)) >> 1) - 1);

  return w;
}

/*
 *   print all subsets of a set of size setsize with elnum elements
 *   does not check  2 ^ setsize possibilities
 */
int riddle3(unsigned int setsize, unsigned int elnum) {
  int count = 0;
  unsigned int sub_num = (1 << setsize);
  unsigned int i =
      (1 << elnum) - 1; // start with elnum 1s in binary, e.g. 111 for elnum=3
  while (i < sub_num) {

    cout << "{ ";
    for (unsigned int j = 1, el = 0; j < sub_num; j *= 2, el++)
      if (j & i) {

        cout << el << " ";
      }
    cout << "}" << endl;
    i = next(i); // get the next number representing subset
    count++;
  }
  return count;
}

int main() {
  unsigned int n, m;
  cin >> n >> m;
  cout << riddle(n) << endl;
  cout << endl;
  cout << riddle2(n, m) << endl;
  cout << endl;
  cout << riddle3(n, m) << endl;
  return 0;
}

/* Note: there exists also a recursive function that realizes this task*/
