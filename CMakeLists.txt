cmake_minimum_required(VERSION 3.20)
project(KirNovEngine C)

set(CMAKE_C_STANDARD 11)

include_directories(lua_src)

file(GLOB LUA_SOURCES "lua_src/*.c")

list(FILTER LUA_SOURCES EXCLUDE REGEX "lua.c$")
list(FILTER LUA_SOURCES EXCLUDE REGEX "luac.c$")

add_executable(KirNovEngine main.c ${LUA_SOURCES})

if(UNIX)
    target_link_libraries(KirNovEngine m)
endif()
