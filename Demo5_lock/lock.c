#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HANDLE open_marks(void)
{
    HANDLE h = CreateFileA("marks.bin", GENERIC_READ | GENERIC_WRITE,
                           FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                           OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE)
        printf("marks.bin: error %lu\n", GetLastError());
    return h;
}

void lock_record(int k, DWORD flags)
{
    HANDLE h = open_marks();
    if (h == INVALID_HANDLE_VALUE)
        return;
    OVERLAPPED ov = { 0 };
    ov.Offset = k * sizeof(int);                 /* звідки блокувати */
    printf("LockFileEx(record %d, bytes %lu..%lu)...\n",
           k, ov.Offset, ov.Offset + 3);
    DWORD start = GetTickCount();
    if (!LockFileEx(h, flags, 0, sizeof(int), 0, &ov)) {
        printf("  error %lu\n", GetLastError());
        CloseHandle(h);
        return;
    }
    printf("  locked after %lu ms. Enter - UnlockFileEx\n", GetTickCount() - start);
    (void)getchar();
    UnlockFileEx(h, 0, sizeof(int), 0, &ov);
    CloseHandle(h);
}

void access_record(int k, int write, int value)
{
    HANDLE h = open_marks();
    if (h == INVALID_HANDLE_VALUE)
        return;
    LARGE_INTEGER pos;
    pos.QuadPart = k * sizeof(int);
    SetFilePointerEx(h, pos, NULL, FILE_BEGIN);
    DWORD n;
    int m = -1;
    BOOL ok = write ? WriteFile(h, &value, sizeof value, &n, NULL)
                    : ReadFile(h, &m, sizeof m, &n, NULL);
    if (!ok)
        printf("%s record %d: error %lu\n", write ? "WriteFile" : "ReadFile", k, GetLastError());
    else if (write)
        printf("WriteFile record %d: ok, %d\n", k, value);
    else
        printf("ReadFile record %d: ok, %d\n", k, m);
    CloseHandle(h);
}

int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("usage: Demo5_lock lock|trylock|read|write k [value]\n");
        return 1;
    }
    int k = atoi(argv[2]);
    if (strcmp(argv[1], "lock") == 0)
        lock_record(k, LOCKFILE_EXCLUSIVE_LOCK);                       /* чекати */
    else if (strcmp(argv[1], "trylock") == 0)
        lock_record(k, LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY);
    else if (strcmp(argv[1], "read") == 0)
        access_record(k, 0, 0);
    else if (strcmp(argv[1], "write") == 0 && argc == 4)
        access_record(k, 1, atoi(argv[3]));
    return 0;
}
