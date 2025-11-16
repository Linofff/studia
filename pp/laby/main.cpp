#include <iostream>
using namespace std;

void f(const char *filename) {
  FILE *f;
  char buff[] = "Hello_darkness_my_old_friend";
  char output[100];
  f = fopen(filename, "a");
  fwrite(buff, sizeof(char), 10, f);
  freopen(filename, "r", f);
  int count = fread(output, sizeof(char), 9, f);
  output[count] = '\0';
  cout << output << endl;
  cout << ftell(f) << endl;
  // rename("test.txt", "plik.txt");
  fclose(f);
}

int main(int argc, char *argv[]) {
  f("test.txt");
  return 0;
}
