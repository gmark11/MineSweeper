/// @file game.c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "game.h"

static void game_over() {}

/**
 * Set the shown property of the cell to true
 * @param c Pointer of the cell
 * @param x The X number of cell
 * @param y The Y number of cell
 */
void show(Cell ***c, int x, int y)
{
    (*c)[x][y].shown = true;
}

/**
 * Set the marked property of the cell to true
 * @param c Pointer of the cell
 * @param x The X number of cell
 * @param y The Y number of cell
 */
void mark(Cell ***c, int x, int y)
{
    (*c)[x][y].marked = true;
}

void save(Game *game, Cell **cells)
{
    FILE *fp = fopen("save.txt", "wt");
    fprintf(fp, "MineSweeper Save\n");
    fprintf(fp, "Game settings\n");
    fprintf(fp, "%d %d\n", game->mode, game->field);
    fprintf(fp, "Cells settings\n");
    for (int i = 0; i < game->field; i++)
    {
        for (int j = 0; j < game->field; j++)
        {
            fprintf(fp, "%d %d %d\n", cells[i][j].type, cells[i][j].shown, cells[i][j].marked);
        }
    }
    //time
    fclose(fp);
}

/*Game load()
{
    FILE *fp = fopen("save.txt", "rt");
    //Map settings
    Game game;
    game.mode =
    game.field =

    //time
    fclose(fp);
}*/

/**
 * Initializes the game
 * @param game Pointer of the game
 * @param mode The mode of the game (enum)
 * @param field The field size of the game (enum)
 */
void new_game(Game *game, GameMode mode, Field field)
{
    game->mode = mode;
    game->field = field;
}

/**
 * Sets the bombs in the cells
 * @param game Pointer of the game
 * @param cells Pointer of the cells
 */
static void set_bombs(Game *game, Cell ***cells)
{
    srand(time(NULL));

    int bomb_cells[game->mode][2];

    //TODO: Do not let it generate more than once the same num!!
    for (int i = 0; i < game->mode; i++)
    {
        bomb_cells[i][0] = rand() % game->field;
        bomb_cells[i][1] = rand() % game->field;
    }

    for (int i = 0; i < game->mode; i++)
    {
        (*cells)[bomb_cells[i][0]][bomb_cells[i][1]].type = bomb;
    }

    //TODO: Search and set cell
    //TODO: Set cell types
}

/**
 * Initializes the cells
 * @param game Pointer of the game
 * @return Cell** Dynamically allocated 2D array
 */
Cell **setup_cells(Game *game)
{
    Cell **cells = (Cell **)malloc(game->field * sizeof(Cell *));

    for (int y = 0; y < game->field; y++)
    {
        cells[y] = (Cell *)malloc(game->field * sizeof(Cell));
    }

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
