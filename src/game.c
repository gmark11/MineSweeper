#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include "game.h"

double get_time(Timer *timer)
{
    if ((timer->game_time / CLOCKS_PER_SEC) > 99.98)
    {
        set_time(timer, 0);
    }
    return timer->game_time / CLOCKS_PER_SEC;
}

// Param: -1: set start_time when the game is started, else: set load time
void set_time(Timer *timer, double t)
{
    if (t == -1)
    {
        timer->start_time = clock();
        timer->game_time = 0;
        timer->loaded_time = 0;
    }
    else
    {
        timer->loaded_time = t * CLOCKS_PER_SEC;
        timer->game_time = 0;
        timer->start_time = 0;
    }
}

void update_time(Timer *timer)
{
    if (timer->loaded_time == 0)
    {
        timer->game_time = clock() - timer->start_time;
    }
    else
    {
        timer->game_time = clock() + timer->loaded_time;
    }
}

void set_status(Status type, Game *game)
{
    game->status = type;
}

void game_over(Game *game, Cell ***cells)
{
    for (int x = 0; x < game->field; x++)
    {
        for (int y = 0; y < game->field; y++)
            (*cells)[x][y].shown = true;
    }
    set_status(gameover, game);
}

void check_win(Game *game, int *covered_cells)
{
    if (*covered_cells == game->mode)
    {
        set_status(win, game);
    }
}

void show(Game *game, Cell ***cells, int x, int y)
{
    if (x < 0 || x >= game->field)
        return;
    if (y < 0 || y >= game->field)
        return;
    if ((*cells)[x][y].marked == true)
        return;
    if ((*cells)[x][y].shown == true)
        return;
    if ((*cells)[x][y].type == bomb)
        game_over(game, cells);
    else
    {
        (*cells)[x][y].shown = true;
        if ((*cells)[x][y].type == simple)
        {
            show(game, cells, x - 1, y);
            show(game, cells, x, y - 1);
            show(game, cells, x, y + 1);
            show(game, cells, x + 1, y);
        }
    }
}

void mark(Cell ***cells, int x, int y)
{
    if ((*cells)[x][y].marked == true)
        (*cells)[x][y].marked = false;
    else
        (*cells)[x][y].marked = true;
}

void save(Game *game, Cell ***cells, Timer *timer)
{
    FILE *fp = fopen("save.txt", "wt");
    if (fp == NULL)
    {
        printf("Error during opening save.txt!");
    }
    //Game settings
    fprintf(fp, "%d %d\n", game->mode, game->field);
    //Time
    fprintf(fp, "%f\n", get_time(timer));
    //Map settings
    for (int i = 0; i < game->field; i++)
    {
        for (int j = 0; j < game->field; j++)
            fprintf(fp, "%d %d %d\n", (*cells)[i][j].type, (*cells)[i][j].shown, (*cells)[i][j].marked);
    }
    fclose(fp);
}

bool load(Game *game, Cell ***cells, Timer *timer)
{
    int num;
    double time_num;

    if (access("save.txt", F_OK) != -1)
    {
        FILE *fp = fopen("save.txt", "rt");
        if (fp == NULL)
        {
            printf("Error during opening save.txt!");
        }

        //Game settings
        fscanf(fp, "%d", &num);
        game->mode = num;

        fscanf(fp, "%d", &num);
        game->field = num;

        //Time
        fscanf(fp, "%lf", &time_num);
        set_time(timer, time_num);

        (*cells) = setup_cells(game); //allocate memory with default settings

        //Map settings
        for (int i = 0; i < game->field; i++)
        {
            for (int j = 0; j < game->field; j++)
            {
                fscanf(fp, "%d", &num);
                (*cells)[i][j].type = num;

                fscanf(fp, "%d", &num);
                (*cells)[i][j].shown = num;

                fscanf(fp, "%d", &num);
                (*cells)[i][j].marked = num;
            }
        }
        fclose(fp);
        remove("save.txt");
        return true;
    }
    else
        return false;
}

void new_game(Game *game, GameMode mode, Field field)
{
    game->mode = mode;
    game->field = field;
    game->status = ingame;
}

void set_type(Game *game, Cell ***cells, int x, int y)
{
    if (x < 0 || x >= game->field)
        return;
    if (y < 0 || y >= game->field)
        return;
    if ((*cells)[x][y].type == three)
        return;
    if ((*cells)[x][y].type == bomb)
        return;

    (*cells)[x][y].type++;
}

void set_bombs(Game *game, Cell ***cells)
{
    srand(time(NULL));

    int x, y;
    int bomb_num = 0;

    while (bomb_num < game->mode)
    {
        x = rand() % game->field;
        y = rand() % game->field;

        if ((*cells)[x][y].type != bomb)
        {
            (*cells)[x][y].type = bomb;
            bomb_num++;

            set_type(game, cells, x - 1, y - 1);
            set_type(game, cells, x - 1, y);
            set_type(game, cells, x - 1, y + 1);
            set_type(game, cells, x, y - 1);
            set_type(game, cells, x, y + 1);
            set_type(game, cells, x + 1, y - 1);
            set_type(game, cells, x + 1, y);
            set_type(game, cells, x + 1, y + 1);
        }
    }
}

Cell **setup_cells(Game *game)
{
    Cell **cells = (Cell **)malloc(game->field * sizeof(Cell *));

    for (int y = 0; y < game->field; y++)
        cells[y] = (Cell *)malloc(game->field * sizeof(Cell));

    for (int i = 0; i < game->field; i++)
    {
        for (int j = 0; j < game->field; j++)
        {
            cells[i][j].type = simple;
            cells[i][j].shown = false;
            cells[i][j].marked = false;
        }
    }
    set_bombs(game, &cells);
    return cells;
}

void free_memory(Cell **cells, Game *game)
{
    for (int y = 0; y < game->field; y++)
        free(cells[y]);
    free(cells);
}
