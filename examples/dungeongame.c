#include "KirNovEngine.h"

int main() {
    keng_init("Dark Dungeon");

    i32 stage = 1;
    i32 running = 1;

    while (running) {
        if (stage == 1) {
            Button room_choice[] = {
                {"Осмотреть тумбочку", 101},
                {"Подойти к двери", 102}
            };

            // Убрали stage из аргументов, теперь их снова 3
            i32 act = menu("КОМНАТА", room_choice, 2);

            if (act == 101) {
                gamelog("Тумбочка пуста.", C_RESET);
            } else if (act == 102) {
                stage = 2;
            }
        }
        else if (stage == 2) {
            Button door_choice[] = {
                {"Выбить дверь", 201},
                {"Назад", 202}
            };

            // И здесь тоже убрали лишний аргумент
            i32 act = menu("ДВЕРЬ", door_choice, 2);

            if (act == 201) {
                gamelog("Свобода!", C_GREEN);
                running = 0;
            } else if (act == 202) {
                stage = 1;
            }
        }
    }
    return 0;
}
