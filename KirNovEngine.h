// KirNovEngine ver 1.0-release

#ifndef KIRNOVENGINE
#define KIRNOVENGINE

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #define CLEAR_CMD "cls"
#else
    #define CLEAR_CMD "clear"
#endif

#define C_RED     "\x1b[31m"
#define C_GREEN   "\x1b[32m"
#define C_YELLOW  "\x1b[33m"
#define C_BLUE    "\x1b[34m"
#define C_RESET   "\x1b[0m"

typedef char* str;
typedef int i32;

typedef struct {
    str label;
    i32 actionID;
} Button;

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
    printf("=== %s ===\n\n", gamename);
}

void gamelog(str log, str color) {
    printf("%s[LOG]: %s%s\n", color, log, C_RESET);
}

// Теперь снова просто 3 аргумента, без указателей и дебага
i32 menu(str title, Button* buttons, i32 count) {
    i32 choice = 0;

    while (1) {
        printf("\n--- %s ---\n", title);
        for (i32 i = 0; i < count; i++) {
            printf("%d. %s\n", i + 1, buttons[i].label);
        }
        printf("------------------\n");
        printf(">> Выбор: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // Очистка при вводе букв
            system(CLEAR_CMD);
            printf("Ошибка: вводи только цифры!\n");
            continue;
        }

        if (choice > 0 && choice <= count) {
            return buttons[choice - 1].actionID;
        } else {
            system(CLEAR_CMD);
            printf("Такого пункта нет!\n");
        }
    }
}

#endif
