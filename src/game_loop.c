#include "game_loop.h"

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

static void init_screen()
{
    freopen("/dev/tty", "r", stdin);
    initscr();                  // screen'i başlatır
    curs_set(0);                // İmleci görünmez yapar
    cbreak();                   // Tuşlar enter beklenmeden direk okunur (Normalde enter'a basılır)
    noecho();                   // Basılan tuşların ekrana basılmasını engeller.
    nodelay(stdscr, TRUE);      // getch normalde bloklar (veri bekler), bloklamadan veri alabilmeyi sağlar
}

void start_game(char** board, int height, int width, int iterations)
{
    init_screen();
    if (has_colors())
    {
        start_color();
        use_default_colors();
        init_pair(PAIR_MESSAGE, COLOR_BLUE, COLOR_BLACK);
        init_pair(NEW_BORN, COLOR_WHITE, -1);
        init_pair(TEEN, COLOR_GREEN, -1);
        init_pair(ADULT, COLOR_YELLOW, -1);
        init_pair(OLD, COLOR_RED, -1);
    }
    else
        return;

    int** alive_log = calloc(height + 1, sizeof(int*));
    if (!alive_log)
        return;

    alive_log[height] = NULL;

    for (int i = 0; i < height; i++)
    {
        alive_log[i] = calloc(width, sizeof(int));
    }

    erase();
    char* message = "Welcome Conyway's Game Of Life";
    for (int i = 0; i < ft_strlen(message); i++)
        mvaddch(0,i+70, message[i] | COLOR_PAIR(PAIR_MESSAGE));

    refresh();
    napms(2000);
    erase();

    for (int i = 0; i < iterations; i++)
    {
        int key = getch();
        if (key == 'Q' || key == 'q')
            break;
        erase();
        for (int j = 0; j < height; j++)
        {
            for (int k = 0; k < width; k++)
            {
                if (board[j][k] == '1')
                {
                    int age = alive_log[j][k]++;

                    if (age == 0)
                        mvaddch(j, k, 'O' | COLOR_PAIR(NEW_BORN));
                    else if (age <= 5)
                        mvaddch(j, k, 'O' | COLOR_PAIR(TEEN));
                    else if (age <= 11)
                        mvaddch(j, k, 'O' | COLOR_PAIR(ADULT));
                    else
                        mvaddch(j, k, 'O' | COLOR_PAIR(OLD));
                }
                else
                    alive_log[j][k] = 0;
  
            }
        }
        refresh();
        napms(100);
        life(board, height, width);
    }
    free_all(alive_log, height);
}