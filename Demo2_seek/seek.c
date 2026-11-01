#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void show(const char *path)
{
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        perror(path);
        return;
    }
    int m;
    printf("%s:", path);
    while (fread(&m, sizeof m, 1, f) == 1)
        printf(" %d", m);
    printf("\n");
    fclose(f);
}

int read_at(FILE *f, long offset, int origin)
{
    int m = -1;
    fseek(f, offset, origin);
    fread(&m, sizeof m, 1, f);
    printf("  fseek(f, %3ld, %s) -> %d, ftell = %ld\n", offset,
           origin == SEEK_SET ? "SEEK_SET" : origin == SEEK_CUR ? "SEEK_CUR" : "SEEK_END",
           m, ftell(f));
    return m;
}

void origins(const char *path)
{
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        perror(path);
        return;
    }
    read_at(f, 2 * sizeof(int), SEEK_SET);       /* оцінка з номером 2 */
    read_at(f, sizeof(int), SEEK_CUR);           /* пропустити одну */
    read_at(f, -(long)sizeof(int), SEEK_END);    /* остання */
    fclose(f);
}

void set_mark(const char *path, int k, int value)
{
    FILE *f = fopen(path, "r+b");                /* читання і запис, без обрізання */
    if (f == NULL) {
        perror(path);
        return;
    }
    fseek(f, k * (long)sizeof(int), SEEK_SET);
    fwrite(&value, sizeof value, 1, f);
    fclose(f);
}

void write_after_read(const char *path, int with_seek)
{
    FILE *f = fopen(path, "r+b");
    if (f == NULL) {
        perror(path);
        return;
    }
    int m, value = 99;
    fread(&m, sizeof m, 1, f);                   /* прочитали оцінку 0 */
    printf("  read %d, ftell = %ld\n", m, ftell(f));
    if (with_seek)
        fseek(f, 0, SEEK_CUR);                   /* стоїмо на місці, але читання -> запис */
    size_t n = fwrite(&value, sizeof value, 1, f);
    printf("  fwrite -> %zu, ftell = %ld\n", n, ftell(f));
    fclose(f);
}

int main(int argc, char *argv[])
{
    if (argc == 5 && strcmp(argv[1], "set") == 0) {
        show(argv[2]);
        set_mark(argv[2], atoi(argv[3]), atoi(argv[4]));
        show(argv[2]);
    } else if (argc == 3 && strcmp(argv[1], "bad") == 0) {
        show(argv[2]);
        write_after_read(argv[2], 0);
        show(argv[2]);
    } else if (argc == 3 && strcmp(argv[1], "good") == 0) {
        show(argv[2]);
        write_after_read(argv[2], 1);
        show(argv[2]);
    } else {
        show("marks.bin");
        origins("marks.bin");
    }
    return 0;
}
