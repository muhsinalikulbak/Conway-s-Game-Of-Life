#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include <ncurses.h>
#include "utils.h"
#include <stdlib.h>

typedef enum 
{ 
    PAIR_MESSAGE = 1,
} ColorPairs;

// maviden kırmızıya, 256 renk paletinden seçilmiş geçiş

static const short gradient[] = {21, 27, 33, 39, 45, 51, 49, 47, 46, 82, 118, 154, 190, 226, 220, 214, 208, 202, 196};
#define GRADIENT_SIZE (sizeof(gradient) / sizeof(gradient[0]))
#define GRADIENT_BASE 10   // pair numaraları 10'dan başlasın

void start_game(char** board, int height, int width, int iterations);

#endif