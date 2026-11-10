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
        init_pair(PAIR_DEAFULT, COLOR_GREEN, COLOR_BLACK);

        if (COLORS >= 256)
        {
            for (unsigned int i = 0; i < GRADIENT_SIZE; i++)
                init_pair(GRADIENT_BASE + i, gradient[i], -1);
        }
    }
    else
        return;

    int alive_cells = 0;
    int color_pair = 1;
    int speed = 100;
    bool gradient_state = true;
    int** age_logs = calloc(height + 1, sizeof(int*));

    if (!age_logs)
        return;
    age_logs[height] = NULL;

    for (int i = 0; i < height; i++)
    {
        age_logs[i] = calloc(width, sizeof(int));
        if (!age_logs[i])
        {
            free_all(age_logs, height);
            return;
        }
    }
    

    erase();
    char* message = "Welcome Conyway's Game Of Life";
    for (int i = 0; i < ft_strlen(message); i++)
    {
        mvaddch(0,i+70, message[i] | COLOR_PAIR(PAIR_MESSAGE));
        refresh();
        napms(100);
    }
    napms(1300);
    erase();
    
    for (int i = 0; i < iterations; i++)
    {
        int key = getch();
        if (key == 'Q' || key == 'q')
            break;
        else if (key == 'C' || key == 'c')
            color_pair = (color_pair + 1) % 7;
        else if (key == '+')
            speed -= 25;
        else if (key == '-')
            speed += 25;
        else if (key == ' ')
            gradient_state = !gradient_state;

        
        erase();
        attron(COLOR_PAIR(PAIR_MESSAGE));
        mvprintw(0, 0, "Iter : %d, Alive Cells : %d, Q: quit, Gradient on/off: SPACE, +: Inc Speed, -: Dec Speed" , i, alive_cells);
        attroff(COLOR_PAIR(PAIR_MESSAGE));

        alive_cells = 0; // Her iterasyonda tekrar hesaplanır.
        for (int j = 0; j < height; j++)
        {
            for (int k = 0; k < width; k++)
            {
                if (board[j][k] == '1')
                {
                    unsigned int age = ++age_logs[j][k];
                    int idx = age < GRADIENT_SIZE ? age : GRADIENT_SIZE - 1;

                    if (gradient_state)
                       mvaddch(j+1, k, 'O' | COLOR_PAIR(GRADIENT_BASE + idx));
                    else
                       mvaddch(j+1, k, 'O' | COLOR_PAIR(PAIR_DEAFULT));

                    alive_cells++;
                }
                else
                    age_logs[j][k] = 0;
            }
        }
        refresh();
        napms(speed);
        life(board, height, width);
    }
}
