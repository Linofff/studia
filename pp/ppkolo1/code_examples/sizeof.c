#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int tab[10] = {0};
    int *wskaznik = &(tab[3]);
    int *alokacja = malloc(2137 * sizeof(int));

    printf("tab[10]: %ld\nwskaznik: %ld\nalokacja: %ld\n", sizeof(tab),
           sizeof(wskaznik), sizeof(alokacja));

    return EXIT_SUCCESS;
}
