#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    printf("Argument | Indeks\n");
    for (int i = 0; i < argc; i++) {
        printf("%-8s | %-6d\n", argv[i], i);
    }
    return EXIT_SUCCESS;
}
