#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    const char *mode = "rb";
    if (argc > 1 && strcmp(argv[1], "text") == 0)
        mode = "r";                              /* без "b" */

    FILE *f = fopen("marks.bin", mode);
    if (f == NULL) {
        perror("marks.bin");
        return 1;
    }
    printf("fopen(\"marks.bin\", \"%s\")\n", mode);

    int buf[4];
    size_t n, total = 0;
    do {
        n = fread(buf, sizeof buf[0], 4, f);     /* до 4 оцінок за раз */
        printf("fread -> %zu:", n);
        for (size_t i = 0; i < n; i++)
            printf(" %d", buf[i]);
        printf("\n");
        total += n;
    } while (n == 4);

    printf("total = %zu marks, feof = %d, ferror = %d\n",
           total, feof(f) != 0, ferror(f) != 0);
    fclose(f);
    return 0;
}
