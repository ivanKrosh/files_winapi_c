#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <string.h>

void print_time(const FILETIME *ft)
{
    SYSTEMTIME utc, local;
    FileTimeToSystemTime(ft, &utc);
    SystemTimeToTzSpecificLocalTime(NULL, &utc, &local);
    printf("%04u-%02u-%02u %02u:%02u:%02u", local.wYear, local.wMonth, local.wDay,
           local.wHour, local.wMinute, local.wSecond);
}

void info(const char *path)
{
    WIN32_FILE_ATTRIBUTE_DATA a;
    if (!GetFileAttributesExA(path, GetFileExInfoStandard, &a)) {
        printf("%s: error %lu\n", path, GetLastError());
        return;
    }
    ULONGLONG size = ((ULONGLONG)a.nFileSizeHigh << 32) | a.nFileSizeLow;
    printf("%s: %llu bytes, changed ", path, size);
    print_time(&a.ftLastWriteTime);
    printf(", read-only: %s\n",
           (a.dwFileAttributes & FILE_ATTRIBUTE_READONLY) ? "yes" : "no");
}

void backup(const char *src, const char *dst)
{
    WIN32_FILE_ATTRIBUTE_DATA s, d;
    if (!GetFileAttributesExA(src, GetFileExInfoStandard, &s)) {
        printf("%s: error %lu\n", src, GetLastError());
        return;
    }
    CreateDirectoryA("backup", NULL);            /* якщо вже є - помилка 183, не страшно */
    if (GetFileAttributesExA(dst, GetFileExInfoStandard, &d) &&
        CompareFileTime(&s.ftLastWriteTime, &d.ftLastWriteTime) <= 0) {
        printf("%s is up to date - skipped\n", dst);
        return;
    }
    if (!CopyFileA(src, dst, FALSE)) {           /* FALSE: перезаписувати можна */
        printf("CopyFile: error %lu\n", GetLastError());
        return;
    }
    printf("%s -> %s copied\n", src, dst);
}

int main(int argc, char *argv[])
{
    const char *cmd = argc > 1 ? argv[1] : "";
    if (strcmp(cmd, "info") == 0 && argc == 3) {
        info(argv[2]);
    } else if (strcmp(cmd, "backup") == 0) {
        backup("marks.bin", "backup\\marks.bin");
    } else if (strcmp(cmd, "copynew") == 0) {
        if (!CopyFileA("marks.bin", "backup\\marks.bin", TRUE))   /* TRUE: не перезаписувати */
            printf("CopyFile(..., TRUE): error %lu\n", GetLastError());
        else
            printf("CopyFile(..., TRUE): ok\n");
    } else if (strcmp(cmd, "move") == 0) {
        if (!MoveFileA("backup\\marks.bin", "backup\\old.bin"))
            printf("MoveFile: error %lu\n", GetLastError());
        else
            printf("MoveFile: backup\\marks.bin -> backup\\old.bin\n");
    } else if (strcmp(cmd, "delete") == 0 && argc == 3) {
        if (!DeleteFileA(argv[2]))
            printf("DeleteFile(%s): error %lu\n", argv[2], GetLastError());
        else
            printf("DeleteFile(%s): ok\n", argv[2]);
    } else {
        printf("usage: Demo6_manage info FILE | backup | copynew | move | delete FILE\n");
    }
    return 0;
}
