#include <iostream>
using namespace std;
#define FEW 3
#define NAME_L 20
#define FILE_NAME_L 20

/*
Complete operations P, S, W, R. They should call functions which save and load
cats from a file using the functions given in the comments.
*/

struct s_cat {
  int birth;
  char name[NAME_L];
};

typedef struct s_cat
    cat; // declare an alias, we no longer need to type struct cat

/*
// the above in one place
typedef struct s_cat {
        int birth;
        char name[NAME_L];
} cat;
*/

void print_cat(cat cat_to_print) {
  cout << cat_to_print.birth << endl;
  cout << cat_to_print.name << endl;
}

void print_cats(cat *clowder, int size) {
  for (int i = 0; i < size; i++) {

    // print_cat(clowder[i]);

    cout << clowder[i].birth << endl;
    cout << (clowder + i)->name << endl;
    // cout<<(*(clowder+i)).name<<endl;
  }
}

void input_cats(cat *clowder, int size) {
  for (int i = 0; i < size; i++) {

    cin >> clowder[i].birth;
    cin >> clowder[i].name;
  }
}

bool print_cats_bin(cat *clowder, int size, char *file_name) {}

int main() {

  /*cat kitten;
  cin>>kitten.birth;
  cin>>kitten.name;

  cout<<kitten.birth<<endl;
  cout<<kitten.name<<endl;
  print_cat(kitten);*/

  cat clowder[FEW];
  /*
  for(int i=0; i<FEW; i++){
          print_cat(clowder[i]);
  }
  cout<<endl;
  */

  char command;
  char name[FILE_NAME_L];
  int quit = 0;
  while (!quit) {

    cin >> command;
    switch (command) {
    case 'I':
      input_cats(clowder, FEW);
      break;
    case 'P': // fprintf
      cin >> name;
      break;
    case 'S': // fscanf
      cin >> name;
      break;
    case 'W': // fwrite
      cin >> name;
      break;
    case 'R': // fread
      cin >> name;
      break;
    case 'Q':
      quit = 1;
      break;
    }
    print_cats(clowder, FEW);
  }
  return 0;
}

/*

1998 Macavity
1981 Gus
1982 Jennyanydots
1939 Old_Deuteronomy
2016 Victoria
2015 Mungojerrie
2014 Rumpleteazer
2019 Jemima
2022 Mistoffelees

*/
