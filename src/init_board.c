
#include <stdlib.h>
#include "init_board.h"
#include "utils.h"
#include <stdio.h>

bool init_board(char** board, int height, int width)
{
    char* actions = read_stdin();
    if (!load_board(board, actions, height, width))
    {
        if (actions)
            free(actions);
        return false;
    }

    if (actions)
        free(actions);
    return true;
}


bool load_board(char** board, char* actions, int row_size, int col_size)
{
    int len = ft_strlen(actions);
    bool draw = false;
    int x = 0; // column
    int y = 0; // row

    for (int i = 0; i < row_size; i++)
    {
        board[i] = calloc(col_size + 1, sizeof(char));
        if (!board[i])
        {
            free_all(board, row_size);
            return false;
        }

        board[i][col_size] = '\0';
        for (int j = 0; j < col_size; j++)
            board[i][j] = '0';
    }
    
    for (int i = 0; i < len; i++)
    {
        char ch = actions[i];
        if (ch != 'w' && ch != 's' && ch != 'd' && ch != 'a' && ch != 'x' && ch != '\n')
        {
            free_all(board, row_size);
            return false;
        }
        
        if (ch == 'w' && y > 0)
            y--;
        else if (ch == 's' && y < row_size - 1)
            y++;
        else if (ch == 'd' && x < col_size - 1)
            x++;
        else if (ch == 'a' && x > 0)
            x--;
        else if (ch == 'x')
            draw = !draw;

        if (draw)
            board[y][x] = '1';
    }
    return true;
}


char* read_stdin()
{
    char buffer[4096];
    char* actions = NULL;
    int byte = read(STDIN_FILENO, buffer, 4095);

    while (byte > 0)
    {
        buffer[byte] = '\0';
        int new_size = byte + ft_strlen(actions);

        char* tmp = calloc(new_size + 1, sizeof(char));
        if (!tmp)
        {
            if (actions)
                free(actions);
            return NULL;
        }
        tmp[new_size] = '\0';

        ft_memcpy(tmp, actions);
        ft_memcpy(tmp + ft_strlen(actions), buffer);
        
        if (actions)
            free(actions);
        actions = tmp;
        byte = read(STDIN_FILENO, buffer, 4095);
    }
    return actions;
}
