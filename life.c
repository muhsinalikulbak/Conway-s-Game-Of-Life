// ./life width height iterations
// $> echo 'sdxddssaaww' | ./a.out 5 5 0 | cat -e
// atoi, read, putchar, malloc, calloc, realloc, free

#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <ncurses.h>

static int ft_strlen(char* str)
{
    if (!str)
        return 0;
    int len = 0;

    while (str[len])
        len++;
    return len;
}


static void life(char** board, int row_size, int col_size)
{
    for (int i = 0; i < row_size; i++)
    {
        bool up = i-1 >= 0;
        bool down = i+1 < row_size;
        for (int j = 0; j < col_size; j++)
        {
            int livingCell = 0;
            bool right = j+1 < col_size;
            bool left = j-1 >= 0;

            if (up)
            {
                if (board[i-1][j] > '0')
                    livingCell++;
                
                if (left && board[i-1][j-1] > '0')
                    livingCell++;
                
                if (right && board[i-1][j+1] > '0')
                    livingCell++;
            }
            if (down)
            {
                if (board[i+1][j] > '0')
                    livingCell++;
                
                if (left && board[i+1][j-1] > '0')
                    livingCell++;
                
                if (right && board[i+1][j+1] > '0')
                    livingCell++;
            }

            if (left && board[i][j-1] > '0')
                livingCell++;
            if (right && board[i][j+1] > '0')
                livingCell++;
            
            if (board[i][j] > '0')
            {
                if (livingCell < 2 || livingCell > 3)
                    board[i][j] = '2';
            }
            else
            {
                if (livingCell == 3)
                    board[i][j] = '/';
            }
        }
    }

    for (int i = 0; i < row_size; i++)
    {
        for (int j = 0; j < col_size; j++)
        {
            if (board[i][j] == '2')
                board[i][j] = '0';
            else if (board[i][j] == '/')
                board[i][j] = '1';
        }
    }
}

static bool load_board(char** board, char* actions, int row_size, int col_size)
{
    int len = ft_strlen(actions);
    bool draw = false;
    int x = 0; // sütun
    int y = 0; // satır

    for (int i = 0; i < row_size; i++)
    {
        board[i] = calloc(sizeof(char), col_size + 1);
        if (!board[i])
        {
            for (int j = i-1; j >= 0; j--)
                if (board[j])
                    free(board[j]);
            return false;
        }

        board[i][col_size] = '\0';
        for (int j = 0; j < col_size; j++)
            board[i][j] = '0';
    }
    
    for (int i = 0; i < len; i++)
    {
        if (actions[i] == 'w' && y > 0)
            y--;
        else if (actions[i] == 's' && y < row_size - 1)
            y++;
        else if (actions[i] == 'd' && x < col_size - 1)
            x++;
        else if (actions[i] == 'a' && x > 0)
            x--;
        else if (actions[i] == 'x')
            draw = !draw;
        
        if (draw)
            board[y][x] = '1';
    }
    return true;
}

static void ft_memcpy(char* dest, char* src)
{
    if (!dest || !src)
        return;
    
    size_t i = 0;
    
    while (src[i])
    {
        dest[i] = src[i];
        i++;
    }
}

static char* read_stdin()
{
    char buffer[4096];
    char* actions = NULL;
    int byte = read(STDIN_FILENO, buffer, 4095);

    while (byte > 0)
    {
        buffer[byte] = '\0';
        int new_size = byte + ft_strlen(actions);

        char* tmp = calloc(sizeof(char), new_size + 1);
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

int main(int argc, char const *argv[])
{
    if (argc != 4)
        return 1;
    
    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    int iterations = atoi(argv[3]);
    
    if (height <= 0 || width <= 0 || iterations < 0)
        return 1;
    
    char* actions = read_stdin();
    char** board = calloc(sizeof(char*), height + 1);
    if (board == NULL)
    {
        if (actions)
            free(actions);
        return 1;
    }
    board[height] = NULL;

    if (!load_board(board, actions, height, width))
    {
        if (actions)
            free(actions);
        if (board)
            free(board);
        return 1;
    }
    if (actions)
        free(actions);

    initscr();
    curs_set(0);
    nodelay(stdscr, TRUE);

    
    // int key = getch();
    // if (key == 'Q' || key == 'q')
    //     break;
    for (int i = 0; i < iterations; i++)
    {
        clear();
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                bool cell = board[i][j] == '1'; 
                mvprintw(i, j, cell ? "O" : " ");
            }
            mvprintw(i+1, 0, "\n");
        }
        refresh();
        napms(50);
        life(board, height, width);
    }

    

    for (int i = 0; i < height; i++)
        if (board[i])
            free(board[i]);

    if (board)
        free(board);
    
    endwin();
    return 0;
}
