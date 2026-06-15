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

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

#define ENG_VER     "1.2-release"

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

// --- МОСТЫ LUA ---

int l_say(lua_State *L) {
    say((str)luaL_checkstring(L, 1), (str)luaL_checkstring(L, 2),
        (str)luaL_checkstring(L, 3), (str)luaL_checkstring(L, 4), (i32)luaL_checkinteger(L, 5));
    return 0;
}

int l_dialogue(lua_State *L) {
    dialogue((str)luaL_checkstring(L, 1), (str)luaL_checkstring(L, 2), (i32)luaL_checkinteger(L, 3));
    return 0;
}

int l_input(lua_State *L) {
    static char buf[256];
    printf("%s", luaL_checkstring(L, 1));
    fflush(stdin);
    if (fgets(buf, sizeof(buf), stdin)) {
        buf[strcspn(buf, "\n")] = 0;
    }
    lua_pushstring(L, buf);
    return 1;
}

int l_menu(lua_State *L) {
    str title = (str)luaL_checkstring(L, 1);
    luaL_checktype(L, 2, LUA_TTABLE);
    int count = (int)lua_rawlen(L, 2);
    Button* btns = malloc(sizeof(Button) * count);
    for (int i = 1; i <= count; i++) {
        lua_rawgeti(L, 2, i);
        btns[i-1].label = strdup(luaL_checkstring(L, -1));
        btns[i-1].actionID = i;
        lua_pop(L, 1);
    }
    i32 res = menu(title, btns, count);
    for(int i=0; i<count; i++) free(btns[i].label);
    free(btns);
    lua_pushinteger(L, res);
    return 1;
}

void register_keng(lua_State *L) {
    lua_register(L, "say", l_say);
    lua_register(L, "dialogue", l_dialogue);
    lua_register(L, "menu", l_menu);
    lua_register(L, "input", l_input);

    lua_pushstring(L, C_RED);     lua_setglobal(L, "C_RED");
    lua_pushstring(L, C_GREEN);   lua_setglobal(L, "C_GREEN");
    lua_pushstring(L, C_YELLOW);  lua_setglobal(L, "C_YELLOW");
    lua_pushstring(L, C_BLUE);    lua_setglobal(L, "C_BLUE");
    lua_pushstring(L, C_CYAN);    lua_setglobal(L, "C_CYAN");
    lua_pushstring(L, C_RESET);   lua_setglobal(L, "C_RESET");

	lua_pushstring(L, RED);     lua_setglobal(L, "RED");
    lua_pushstring(L, GREEN);   lua_setglobal(L, "GREEN");
    lua_pushstring(L, YELLOW);  lua_setglobal(L, "YELLOW");
    lua_pushstring(L, BLUE);    lua_setglobal(L, "BLUE");
    lua_pushstring(L, CYAN);    lua_setglobal(L, "CYAN");
    lua_pushstring(L, RESET);   lua_setglobal(L, "RESET");
}

// --- MAIN (С поддержкой перетаскивания файлов) ---

int main(int argc, char* argv[]) {
    str script_file = "main.lua";
    if (argc > 1) script_file = argv[1];

    keng_init("KirNovEngine Player Ver 1.0");

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    register_keng(L);

    printf(C_YELLOW "Running: %s\n\n" C_RESET, script_file);

    if (luaL_dofile(L, script_file) != LUA_OK) {
        printf(C_RED "Lua Error: %s\n" C_RESET, lua_tostring(L, -1));
    }

    lua_close(L);
    printf("\n--------------------------\nDone. Press Enter...");
    getchar(); getchar();
    return 0;
}
