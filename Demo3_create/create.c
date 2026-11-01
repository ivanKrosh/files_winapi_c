#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reset(void)                                 /* exists.bin - 40 байтів, none.bin - немає */
{
    int zeros[10] = { 0 };
    FILE *f = fopen("exists.bin", "wb");
    fwrite(zeros, sizeof zeros[0], 10, f);
    fclose(f);
    remove("none.bin");
}

void try_open(const char *path, DWORD disposition)
{
    HANDLE h = CreateFileA(path, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                           disposition, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD err = GetLastError();
    char cell[32];
    if (h == INVALID_HANDLE_VALUE) {
        sprintf(cell, "error %lu", err);
    } else {
        LARGE_INTEGER size;
        GetFileSizeEx(h, &size);
        sprintf(cell, "ok, %lld B, last %lu", size.QuadPart, err);
        CloseHandle(h);
    }
    printf("  %-22s", cell);
}

void modes(void)
{
    const char *names[] = { "CREATE_NEW", "CREATE_ALWAYS", "OPEN_EXISTING",
                            "OPEN_ALWAYS", "TRUNCATE_EXISTING" };
    DWORD values[] = { CREATE_NEW, CREATE_ALWAYS, OPEN_EXISTING,
                       OPEN_ALWAYS, TRUNCATE_EXISTING };
    printf("%-18s  %-22s  %s\n", "disposition", "exists.bin (40 B)", "none.bin");
    for (int i = 0; i < 5; i++) {
        printf("%-18s", names[i]);
        reset();
        try_open("exists.bin", values[i]);
        try_open("none.bin", values[i]);
        printf("\n");
    }
    reset();
}

void read_file(void)
{
    HANDLE h = CreateFileA("marks.bin", GENERIC_READ, FILE_SHARE_READ, NULL,
                           OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) {
        printf("marks.bin: error %lu\n", GetLastError());
        return;
    }
    int buf[4];
    DWORD got;
    BOOL ok;
    do {
        ok = ReadFile(h, buf, sizeof buf, &got, NULL);
        printf("ReadFile -> %s, got %2lu bytes\n", ok ? "TRUE" : "FALSE", got);
    } while (ok && got > 0);                     /* TRUE і 0 байтів - кінець файлу */

    LARGE_INTEGER move, pos;
    move.QuadPart = -(LONGLONG)sizeof(int);
    SetFilePointerEx(h, move, &pos, FILE_END);   /* остання оцінка */
    ReadFile(h, buf, sizeof(int), &got, NULL);
    printf("SetFilePointerEx(-4, FILE_END) -> position %lld, mark %d\n",
           pos.QuadPart, buf[0]);
    CloseHandle(h);
}

void append(const char *path, int value)
{
    HANDLE h = CreateFileA(path, GENERIC_WRITE, 0, NULL,
                           OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) {
        printf("%s: error %lu\n", path, GetLastError());
        return;
    }
    LARGE_INTEGER zero = { 0 }, pos, size;
    SetFilePointerEx(h, zero, &pos, FILE_END);   /* у кінець файлу */
    DWORD written;
    WriteFile(h, &value, sizeof value, &written, NULL);
    GetFileSizeEx(h, &size);
    printf("WriteFile at %lld: %lu bytes, size now %lld\n",
           pos.QuadPart, written, size.QuadPart);
    CloseHandle(h);
}

int main(int argc, char *argv[])
{
    if (argc > 1 && strcmp(argv[1], "read") == 0)
        read_file();
    else if (argc == 4 && strcmp(argv[1], "append") == 0)
        append(argv[2], atoi(argv[3]));
    else
        modes();
    return 0;
}
