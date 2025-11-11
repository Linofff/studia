#include <iostream>
using namespace std;

typedef struct A {
  char c;
  struct A *next;
} list_t;

list_t *add(list_t *h, char c) {
  list_t *n = (list_t *)malloc(sizeof(list_t));
  n->next = h;
  n->c = c;
  return n;
}

int main() {
  list_t *l = NULL;

  l = add(l, 'A');
  l->next = add(l, 'B');

  while (l != NULL) {
    cout << l->c;
    l = l->next;
  }

  return 0;
}
