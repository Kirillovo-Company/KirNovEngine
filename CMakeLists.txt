cmake_minimum_required(VERSION 3.20)
project(KirNovEngineNEXT C)

set(CMAKE_C_STANDARD 11)

# 1. Говорим, где искать заголовочные файлы (.h)
include_directories(lua_src)

# 2. Находим все файлы .c в папке lua_src
file(GLOB LUA_SOURCES "lua_src/*.c")

# 3. Убираем из списка lua.c и luac.c (в них свои main, они мешают)
list(FILTER LUA_SOURCES EXCLUDE REGEX "lua.c$")
list(FILTER LUA_SOURCES EXCLUDE REGEX "luac.c$")

# 4. Собираем проект: твой main.c и очищенный список файлов Lua
add_executable(KirNovEngineNEXT main.c ${LUA_SOURCES})

# Если используешь Windows, иногда нужно явно линковать библиотеку m (математика)
if(UNIX)
    target_link_libraries(KirNovEngineNEXT m)
endif()