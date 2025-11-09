#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char *napis = malloc(30 * sizeof(char));
    if (napis == NULL) {
        exit(EXIT_FAILURE);
    }

    // Można nawiasami kwadratowymi
    napis[0] = 'O';
    napis[1] = 'b';
    napis[2] = 's';
    napis[3] = 'z';
    napis[4] = 'a';

    // Można też dereferencją
    *(napis + 5) = 'r';
    *(napis + 6) = '\n';
    *(napis + 7) = '\0';

    printf("%s", napis);

    // Oddajemy pamięć
    free(napis);
    return EXIT_SUCCESS;
}
