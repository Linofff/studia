#include <iostream>
using namespace std;
int x;
int main() {
    int x[] = {1, 4, 7, 9, 17};
    int *p = &(x[2]);
    cout << p << '\n';
    cout << (sizeof x) / (sizeof(*(p--))) << '\n'; /* I-1 */
    cout << p << '\n';
    cout << (p - x - 2) / (1 + sizeof x[0]) << '\n'; /* I-2 */
    return 0;
}
