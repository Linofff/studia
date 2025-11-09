#include <stdio.h>
#include <stdlib.h>

#define XD "crazy"
#define zmienna wartosc

#undef int
#define int char

int main() {
    int *napis = "fajny napis";
    long wartosc = 13;
    printf("%s %s %ld\n", napis, XD, zmienna);

    return EXIT_SUCCESS;
}
