#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include <ncurses.h>
#include "utils.h"
#include <stdlib.h>

typedef enum 
{ 
    PAIR_ALIVE_CELL = 1,
    PAIR_DEFAULT = 2,
    NEW_BORN = 3,
    PAIR_MESSAGE = 4,
    TEEN = 5,
    ADULT = 6,
    OLD = 7
} ColorPairs;

void start_game(char** board, int height, int width, int iterations);

#endif