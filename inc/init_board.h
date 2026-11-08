#ifndef INIT_BOARD_H
#define INIT_BOARD_H
#include <stdbool.h>
#include <unistd.h>

bool load_board(char** board, char* actions, int row_size, int col_size);
char* read_stdin();
bool init_board(char** board, int height, int width);



#endif