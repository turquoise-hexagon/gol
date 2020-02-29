#include <err.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char** argv)
{
    /* argument parsing */
    if (argc != 3) {
        fprintf(stderr, "usage: %s [rule] [strip]\n", argv[0]);

        return 1;
    }

    errno = 0;
    char* ptr;

    long rule = strtol(argv[1], &ptr, 10);

    if (errno != 0 || *ptr != 0 || rule < 0 || rule > 255)
        errx(1, "'%s' isn't a valid rule", argv[1]);

    unsigned length = strlen(argv[2]);

    /* init arrays */
    bool* origin = malloc((length + 2) * sizeof(bool));
    bool* backup = malloc((length + 2) * sizeof(bool));

    if (origin == NULL || backup == NULL)
        errx(1, "program failed to allocate memory");

    origin[0] = origin[length] = false;
    backup[0] = backup[length] = false;

    for (unsigned i = 1; i <= length; i++)
        switch (argv[2][i - 1]) {
            case '0' : origin[i] = false; break;
            case '1' : origin[i] = true;  break;
            default  :
                free(origin);
                free(backup);

                errx(1, "'%s' isn't a valid strip", argv[2]);
        }

    /* run cellular automaton */
    printf("P1\n%d %d\n", length, length);

    for (unsigned n = 0; n < length; n++) {
        for (unsigned tmp = 0, i = 1; i <= length; tmp = 0, i++) {
            putchar(origin[i] == false ? '0' : '1');

            for (short j = -1; j < 2; j++)
                tmp = tmp << 1 | origin[i + j];

            backup[i] = 1 & rule >> tmp;
        }

        for (unsigned i = 1; i <= length; i++)
            origin[i] = backup[i];
    }

    /* cleanup */
    free(origin);
    free(backup);

    return 0;
}
