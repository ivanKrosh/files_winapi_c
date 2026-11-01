# Основні функції для роботи з файлами — демонстрації

Демонстраційні програми до заняття 12 курсу «Системне програмування»
(ВСП «Фаховий коледж інформаційних систем і технологій КНЕУ ім. Вадима Гетьмана»).

Тема заняття — **файл — це не лише потік байтів від початку до кінця**: читання двійкового файлу (`fread`), прямий доступ (`fseek`), відкриття функцією `CreateFile` з потрібною поведінкою для наявного файлу, `ReadFile` / `WriteFile` / `SetFilePointerEx`, спільний доступ, блокування ділянки файлу (`LockFileEx`), інформація про файл, копіювання, перенесення й видалення.

Усі програми працюють з одним файлом — `Файли/marks.bin`.

## Склад рішення

Рішення `Demo_Lecture_12.slnx`: шість проєктів на C, тека зі сценаріями командного рядка і тека `Файли` з даними.

| Проєкт | Файл | Розділ конспекту | Що показує |
|---|---|---|---|
| `Demo1_fread` | `fread.c` | 1 | `fread` порціями по 4 оцінки: 4, 4, 2; аргумент `text` — той самий файл без `"b"` обривається на байті `1A` (Ctrl+Z) |
| `Demo2_seek` | `seek.c` | 2 | `fseek` від початку, від поточного місця і від кінця; `set` — зміна оцінки на місці в режимі `"r+b"`; `bad` / `good` — запис після читання без `fseek` і з ним |
| `Demo3_create` | `create.c` | 3 | п'ять значень `dwCreationDisposition` для наявного і відсутнього файлу; `read` — `ReadFile` до TRUE і 0 байтів, `SetFilePointerEx`; `append` — `WriteFile` у кінець |
| `Demo4_share` | `share.c` | 4 | `hold` / `holdread` тримає файл відкритим з `dwShareMode` 0 або `FILE_SHARE_READ`; `read`, `write`, `fopen` — спроби іншого процесу (помилка 32, `errno` 13) |
| `Demo5_lock` | `lock.c` | 5 | `lock k` / `trylock k` — `LockFileEx` на одну оцінку; `read k`, `write k v` — доступ іншого процесу (помилка 33) |
| `Demo6_manage` | `manage.c` | 6 | `info` — `GetFileAttributesEx`; `backup` — `CompareFileTime` + `CopyFile`; `copynew` — `CopyFile(…, TRUE)`; `move` — `MoveFile`; `delete` — `DeleteFile` |

| Сценарій | Порядок на занятті | Програма |
|---|---|---|
| `Сценарії/demo01_fread.cmd` | 1 | `Demo1_fread` |
| `Сценарії/demo02_seek.cmd` | 2 | `Demo2_seek` |
| `Сценарії/demo03_create.cmd` | 3 | `Demo3_create` |
| `Сценарії/demo04_share.cmd` | 4 | `Demo4_share` — відкриває друге вікно |
| `Сценарії/demo05_lock.cmd` | 5 | `Demo5_lock` — відкриває друге вікно |
| `Сценарії/demo06_manage.cmd` | 6 | `Demo6_manage` |

Кожен проєкт показується **один раз**, у порядку номерів.

Сценарії запускають програми з `x64\Debug\`, тому **зберіть рішення в Debug**. Поточним каталогом сценарії роблять теку `Файли`.

## Тека `Файли`

У репозиторії в ній лише **`marks.bin`** — 40 байтів, десять оцінок типу `int`: `74 58 91 66 45 83 60 97 26 72`.

Сценарії `marks.bin` не змінюють: зміни робляться в копіях `fix.bin`, `bad.bin`, `good.bin`, `copy.bin`. `Demo3_create` створює й видаляє `exists.bin` і `none.bin`, `Demo6_manage` — теку `backup` (сценарій видаляє її на початку). Git ігнорує всі ці файли. Сценарій `demo06_manage.cmd` оновлює **час зміни** `marks.bin`, вміст лишається тим самим.

Якщо `marks.bin` усе ж зіпсовано: `git checkout Файли/marks.bin`.

## Як відкрити у Visual Studio

**Що має бути встановлено.** Visual Studio (Community достатньо) з компонентом **«Розробка класичних застосунків на C++»** (Desktop development with C++).

**Крок 1. Відкрити рішення.** Двічі клацнути `Demo_Lecture_12.slnx` або *File → Open → Project/Solution*.

> Файл рішення збережено в новому форматі `.slnx`. Якщо ваша версія Visual Studio його не відкриває, оновіть її або відкрийте потрібний `.vcxproj` напряму.

**Крок 2. Зібрати.** **Debug** і **x64**, *Build → Build Solution* (Ctrl+Shift+B).

**Крок 3. Запускати.** `Ctrl+F5` — без налагоджувача, `F5` — під налагоджувачем. Зручно один раз налаштувати: правою кнопкою на рішенні → *Properties* → *Startup Project* → **Current selection**. Аргументи командного рядка задаються в *Properties → Configuration Properties → Debugging → Command Arguments*. *Working Directory* у всіх проєктах — `$(SolutionDir)Файли\`.

Демонстрації 4 і 5 потребують **двох процесів одночасно**. Зручніше запускати їх сценаріями: вони відкривають друге вікно командою `start`.

## Налаштування проєктів

Ті самі, що в демонстраціях до занять 8–11: `/W4`, `/utf-8`, Debug без *Just My Code* і без *Basic Runtime Checks*. У кожному файлі перший рядок — `#define _CRT_SECURE_NO_WARNINGS`. Функції WinAPI викликаються у варіанті `A` (`CreateFileA`, `CopyFileA`): імена файлів у демонстраціях латинські.

## Як зібрати з командного рядка

**Developer Command Prompt for VS**, з кореня репозиторію:

```
msbuild Demo_Lecture_12.slnx /p:Configuration=Debug /p:Platform=x64
```

Окремий файл:

```
cl /nologo /utf-8 /W4 Demo3_create\create.c       -> create.exe
```

## Як запускати без сценаріїв

З теки `Файли`:

```
..\x64\Debug\Demo1_fread.exe
..\x64\Debug\Demo1_fread.exe text               без "b"
..\x64\Debug\Demo2_seek.exe
copy marks.bin fix.bin  &  ..\x64\Debug\Demo2_seek.exe set fix.bin 8 62
copy marks.bin bad.bin  &  ..\x64\Debug\Demo2_seek.exe bad bad.bin
copy marks.bin good.bin &  ..\x64\Debug\Demo2_seek.exe good good.bin
..\x64\Debug\Demo3_create.exe
..\x64\Debug\Demo3_create.exe read
copy marks.bin copy.bin &  ..\x64\Debug\Demo3_create.exe append copy.bin 88
..\x64\Debug\Demo4_share.exe hold               у першому вікні
..\x64\Debug\Demo4_share.exe read               у другому: read | write | fopen
..\x64\Debug\Demo5_lock.exe lock 8              у першому вікні
..\x64\Debug\Demo5_lock.exe read 8              у другому: read k | write k v | trylock k | lock k
..\x64\Debug\Demo6_manage.exe info marks.bin
..\x64\Debug\Demo6_manage.exe backup            далі: copynew | move | delete FILE
```

## Відомі граблі

**`Demo2_seek bad` у Debug показує вікно *Debug Assertion Failed*.** Так і має бути: Debug-бібліотека C ловить запис після читання без `fseek` («Flush between consecutive read and write»). Натисніть **Ignore** — програма продовжить, і `fwrite` поверне 0. У Release вікна немає, лише 0.

**Друге вікно в демо 4 і 5 не закривається.** Воно чекає на Enter: програма тримає файл відкритим або заблокованим, доки ви його не натиснете.

**`Demo5_lock lock 8` у першому вікні «зависає».** Так і має бути: `LockFileEx` без `LOCKFILE_FAIL_IMMEDIATELY` чекає, доки друге вікно зніме блокування. Натисніть Enter у другому вікні.

**Дата й час у `Demo6_manage info` не збігаються з конспектом.** Так і має бути: це час зміни файлу на вашому диску.

**Кодування сценаріїв.** Файли `.cmd` збережено в **UTF-8 з CRLF**, перший рядок після `@echo off` — `chcp 65001 > nul`.

## Супровідні матеріали

Конспект заняття та презентація зберігаються в навчально-методичному комплексі курсу.
