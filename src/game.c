#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include "debugmalloc.h"
#include "game.h"

Status STATUS = ingame;

void set_status(Status type)
{
    STATUS = type;
}

Status get_status()
{
    return STATUS;
}

void game_over(Game *game, Cell ***cells)
{
    for (int x = 0; x < game->field; x++)
    {
        for (int y = 0; y < game->field; y++)
        {
            (*cells)[x][y].shown = true;
        }
    }
    set_status(gameover);
}

void check_win(Game *game, Cell ***cells) //TODO
{
    int active = 0;
    for (int x = 0; x < game->field; x++)
    {
        for (int y = 0; y < game->field; y++)
        {
            if ((*cells)[x][y].shown == true)
            {
                active++;
            }
        }
    }
    if (active == game->mode)
    {
        set_status(win);
    }
}

void show(Game *game, Cell ***cells, int x, int y)
{
    if (x < 0 || x >= game->field)
        return;
    if (y < 0 || y >= game->field)
        return;
    if ((*cells)[x][y].shown == true)
        return;
    if ((*cells)[x][y].marked == true)
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
    //check_win(game, cells);
}

void mark(Cell ***cells, int x, int y)
{
    (*cells)[x][y].marked = true;
}

void save(Game *game, Cell ***cells)
{
    FILE *fp = fopen("save.txt", "wt");
    //Game settings
    fprintf(fp, "%d %d\n", game->mode, game->field);
    //Map settings
    for (int i = 0; i < game->field; i++)
    {
        for (int j = 0; j < game->field; j++)
        {
            fprintf(fp, "%d %d %d\n", (*cells)[i][j].type, (*cells)[i][j].shown, (*cells)[i][j].marked);
        }
    }
    //TODO: time
    fclose(fp);
}

bool load(Game *game, Cell ***cells)
{
    int num;
    char new_line;

    if (access("save.txt", F_OK) != -1)
    {
        FILE *fp = fopen("save.txt", "rt");
        //Game settings
        fscanf(fp, "%d", &num);
        game->mode = num;
        fscanf(fp, "%c", &new_line); //new line char

        fscanf(fp, "%d", &num);
        game->field = num;
        fscanf(fp, "%c", &new_line); //new line char

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

                fscanf(fp, "%c", &new_line); //new line char
            }
        }
        //TODO: time
        fclose(fp);
        remove("save.txt");
        return true;
    }
    else
    {
        return false;
    }
}

void new_game(Game *game, GameMode mode, Field field)
{
    game->mode = mode;
    game->field = field;
}

void set_type(Game *game, Cell ***cells, int x, int y)
{
    if (x < 0 || x >= game->field)
        return;
    if (y < 0 || y >= game->field)
        return;
    if ((*cells)[x][y].type == bomb)
        return;
    if ((*cells)[x][y].type == three)
        return;

    (*cells)[x][y].type++;
}

void set_bombs(Game *game, Cell ***cells)
{
    srand(time(NULL));

    int x, y, n;

    n = 0;
    while (n < game->mode)
    {
        x = rand() % game->field;
        y = rand() % game->field;

        if ((*cells)[x][y].type != bomb)
        {
            (*cells)[x][y].type = bomb;
            set_type(game, cells, x - 1, y - 1);
            set_type(game, cells, x - 1, y);
            set_type(game, cells, x - 1, y + 1);
            set_type(game, cells, x, y - 1);
            set_type(game, cells, x, y + 1);
            set_type(game, cells, x + 1, y - 1);
            set_type(game, cells, x + 1, y);
            set_type(game, cells, x + 1, y + 1);
            n++;
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
