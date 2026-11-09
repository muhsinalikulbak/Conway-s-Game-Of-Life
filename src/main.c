// ./life width height iterations
// $> echo 'sdxddssaaww' | ./a.out 5 5 0
// atoi, read, putchar, malloc, calloc, realloc, free

#include <stdlib.h>
#include <unistd.h>
#include "init_board.h"
#include "game_loop.h"

int main(int argc, char const *argv[])
{
    if (argc != 4)
        return 1;
    
    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    int iterations = atoi(argv[3]);
    
    if (height <= 0 || width <= 0 || iterations < 0)
        return 1;

    char** board = calloc(sizeof(char*), height + 1);
    if (board == NULL)
        return 1;
    board[height] = NULL;
    
    if (!init_board(board, height, width))
        return 1;

    start_game(board, height, width, iterations);


    free_all(board, height);
    endwin();
    
    return 0;
}
