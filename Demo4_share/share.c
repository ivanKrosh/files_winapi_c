#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

void hold(DWORD share)
{
    HANDLE h = CreateFileA("marks.bin", GENERIC_READ | GENERIC_WRITE, share, NULL,
                           OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) {
        printf("marks.bin: error %lu\n", GetLastError());
        return;
    }
    printf("marks.bin is open, share = %s\n",
           share == 0 ? "0" : "FILE_SHARE_READ");
    printf("Enter - CloseHandle\n");
    (void)getchar();
    CloseHandle(h);
}

void try_open(DWORD access)
{
    HANDLE h = CreateFileA("marks.bin", access, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                           OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    const char *name = access == GENERIC_READ ? "GENERIC_READ" : "GENERIC_WRITE";
    if (h == INVALID_HANDLE_VALUE) {
        printf("CreateFile(%s): error %lu\n", name, GetLastError());
        return;
    }
    printf("CreateFile(%s): ok\n", name);
    CloseHandle(h);
}

void try_fopen(void)
{
    FILE *f = fopen("marks.bin", "rb");
    if (f == NULL) {
        perror("fopen(\"marks.bin\", \"rb\")");
        printf("  errno = %d\n", errno);
        return;
    }
    printf("fopen(\"marks.bin\", \"rb\"): ok\n");
    fclose(f);
}

int main(int argc, char *argv[])
{
    const char *cmd = argc > 1 ? argv[1] : "";
    if (strcmp(cmd, "hold") == 0)
        hold(0);                                 /* нікого не пускати */
    else if (strcmp(cmd, "holdread") == 0)
        hold(FILE_SHARE_READ);                   /* пускати лише читачів */
    else if (strcmp(cmd, "read") == 0)
        try_open(GENERIC_READ);
    else if (strcmp(cmd, "write") == 0)
        try_open(GENERIC_WRITE);
    else if (strcmp(cmd, "fopen") == 0)
        try_fopen();
    else
        printf("usage: Demo4_share hold | holdread | read | write | fopen\n");
    return 0;
}
