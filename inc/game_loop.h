#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include <ncurses.h>
#include "utils.h"
#include <stdlib.h>

typedef enum 
{ 
    PAIR_DEFAULT = 1,
    PAIR_ALIVE_CELL = 2,
    PAIR_DEAD_CELL = 3,
    PAIR_BORDER = 4,
    NEW_BORN = 5,
    TEEN = 6,
    ADULT = 7,
    OLD = 8,
    PAIR_MESSAGE
} ColorPairs;

void start_game(char** board, int height, int width, int iterations);

#endif