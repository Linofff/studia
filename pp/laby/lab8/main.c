#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define name_max_length 25

typedef struct {
  char name[name_max_length + 1];
  int crew;
} Ship;

typedef struct {
  int id;
  Ship *theirship;
} Person;

Ship ship[10];
int currship = -1;

void komenda_D(Person *people, int n) {
  char name_buffer[name_max_length];

  scanf("%s", name_buffer);

  currship++;
  strcpy(ship[currship].name, name_buffer);
}
void komenda_B(Person *people, int n) {
  int id_number;
  int searched_index;
  int found = 0;
  scanf("%d", &id_number);
  for (int i = 0; i < n; i++) {
    if (people[i].id == id_number) {
      searched_index = i;
      found = 1;
      break;
    }
  }
  if (found) {
    people[searched_index].theirship = &ship[currship];
    ship[currship].crew++;

  } else {
    printf("Wrong Id \n");
  }
}

void komenda_P(Person *people, int n) {
  printf("Log:\n");
  for (int i = 0; i < n; i++) {
    if (people[i].theirship != NULL) {
      printf("Person %d:\t%s, crew: %d\n", people[i].id,
             people[i].theirship->name, people[i].theirship->crew);

    } else {
      printf("Person %d:\t-\n", people[i].id);
    }
  }
}

int main() {
  int n;
  scanf("%d", &n);
  Person *people = (Person *)malloc(n * sizeof(Person));

  for (int i = 0; i < n; i++) {
    scanf("%d", &people[i].id);
    people[i].theirship = NULL;
  }

  char komenda;
  while (scanf(" %c", &komenda) != EOF) {
    if (komenda == 'Q') {
      break;

    } else if (komenda == 'D') {
      komenda_D(people, n);

    } else if (komenda == 'B') {
      komenda_B(people, n);

    } else if (komenda == 'P') {
      komenda_P(people, n);
    }
  }

  return 0;
}
