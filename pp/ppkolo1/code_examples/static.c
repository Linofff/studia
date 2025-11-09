#include <stdio.h>
#include <stdlib.h>

void zliczaj();

int main(int argc, char *argv[]) {
    for (int i = 0; i < 3; i++) {
        zliczaj();
    }

    return EXIT_SUCCESS;
}

void zliczaj() {
    static int liczba = 0;
    int zwykla = 0;

    liczba++;
    zwykla++;

    printf("Static: %d\n", liczba);
    printf("Zwykła: %d\n", zwykla);
}
