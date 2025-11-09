#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int a[10] = {1, 4, 5, 6, 3, 4, 5, 3, 1, 3};

    printf("Wartość    | Adres          | Indeks\n");

    char *dopisek = " <- na to pokazuje wskaźnik a\n";
    char *dopisek_default = "\n";
    for (int i = 0; i < 10; i++) {
        char *ending = i == 0 ? dopisek : dopisek_default;

        printf("%-10d | %-10p | %-d %s", a[i], &a[i], i, ending);
    }

    return EXIT_SUCCESS;
}
