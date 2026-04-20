// KirNovEngine 1.1-release

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Подключаем Lua
#include "lua_src/lua.h"
#include "lua_src/lualib.h"
#include "lua_src/lauxlib.h"

// --- СЕКЦИЯ ДВИЖКА KIRNOVENGINE ---

#ifdef _WIN32
    #include <windows.h>
    #define CLEAR_CMD "cls"
#else
    #include <unistd.h>
    #define CLEAR_CMD "clear"
#endif

#define C_RED     "\x1b[31m"
#define C_GREEN   "\x1b[32m"
#define C_YELLOW  "\x1b[33m"
#define C_BLUE    "\x1b[34m"
#define C_MAGENTA "\x1b[35m"
#define C_CYAN    "\x1b[36m"
#define C_RESET   "\x1b[0m"

#define ENG_VER     "1.1-release"

typedef char* str;
typedef int i32;

typedef struct {
    str label;
    i32 actionID;
} Button;

void dsleep(i32 ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

void keng_init(str gamename) {
#ifdef _WIN32
    system("chcp 65001 > nul");
    char cmd[128];
    sprintf(cmd, "title %s", gamename);
    system(cmd);
#else
    printf("\033]0;%s\007", gamename);
#endif
    system(CLEAR_CMD);
    printf("=== %s ===\n", gamename);
    printf(C_CYAN "KirNovEngine ver: %s\n" C_RESET, ENG_VER);
    printf("--------------------------\n\n");
}

void say(str name, str name_color, str text, str text_color, i32 speed_ms) {
    printf("%s[%s]%s: ", name_color, name, C_RESET);
    printf("%s", text_color);
    for (i32 i = 0; text[i] != '\0'; i++) {
        printf("%c", text[i]);
        fflush(stdout);
        dsleep(speed_ms);
    }
    printf("%s\n", C_RESET);
}

void dialogue(str text, str color, i32 speed_ms) {
    printf("%s", color);
    for (i32 i = 0; text[i] != '\0'; i++) {
        printf("%c", text[i]);
        fflush(stdout);
        dsleep(speed_ms);
    }
    printf("%s\n", C_RESET);
}

i32 menu(str title, Button* buttons, i32 count) {
    i32 choice = 0;
    while (1) {
        printf("\n--- %s ---\n", title);
        for (i32 i = 0; i < count; i++) {
            printf("%d. %s\n", i + 1, buttons[i].label);
        }
        printf("------------------\n>> Choice: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            system(CLEAR_CMD);
            printf(C_RED "Error: only numbers!\n" C_RESET);
            continue;
        }
        if (choice > 0 && choice <= count) return buttons[choice - 1].actionID;
        else {
            system(CLEAR_CMD);
            printf(C_YELLOW "Choice does not exist!\n" C_RESET);
        }
    }
}
