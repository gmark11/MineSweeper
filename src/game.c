#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "game.h"

/**
 * Marks the selected cell.
 * @param c A The clicked cell.
 * @return true, ha sikeres volt a mentés.
 */
static void game_over()
{
}

/**
 * Shows the selected cell.
 * @param c A The clicked cell.
 * @return true, ha sikeres volt a mentés.
 */
static int show(Cell *c)
{
    c->shown = true;
    if (c->type == bomb)
    {
        game_over();
    }
    else
    {
        switch (c->type)
        {
        case one:
            //
            break;
        case two:
            //
            break;
        case three:
            //
            break;
        case four:
            //
            break;
        case five:
            //
            break;
        }
    }
    return 0;
}

/**
 * Marks the selected cell.
 * @param c A The clicked cell.
 * @return true, ha sikeres volt a mentés.
 */
static int mark(Cell *c)
{
    c->marked = true;
}

/**
 * Handles mouse clicks (left or right).
 * @param c The clicked cell.
 * @param mouse The type of the mouse click (left or right).
 * @return void
 */
void click(Cell *c, Click mouse)
{
    if (mouse == left)
    {
        show(c);
    }
    if (mouse == right)
    {
        mark(c);
    }
}

/**
 * Saves current status to a txt file.
 * @param pj A mentendő játék.
 * @param fajlnev A fájl neve, amit létrehoz.
 * @return 0, if successful save
 */
int save()
{
    FILE *fp = fopen("save.txt", "wt");
    fprintf(fp, "MineSweeper Save\n");
    //Map settings
    //Current status
    fclose(fp);
    return 0;
}

/**
 * Elmenti a játékot egy fájlba.
 * @param pj A mentendő játék.
 * @param fajlnev A fájl neve, amit létrehoz.
 * @return true, ha sikeres volt a mentés.
 */
int load_game()
{
}

/**
 * Elmenti a játékot egy fájlba.
 * @param pj A mentendő játék.
 * @param fajlnev A fájl neve, amit létrehoz.
 * @return true, ha sikeres volt a mentés.
 */
int new_game()
{
}
/*
Cell* set_bombs(int x, int y, int bomb_num, Cell *cells){
    srand(time(NULL));

    int bomb_cells[bomb_num][2];

    //TODO: Do not let it generate more than once the same num!!
    for(int i=0; i<bomb_num; i++){
        bomb_cells[i][0] = rand() % x;
        bomb_cells[i][1] = rand() % y;
    }

    for(int i=0; i<bomb_num; i++){
        cells[bomb_cells[i][0]][bomb_cells[i][1]].type = bomb;
    }

    //Search and set cell
    return *cells;
}


/**
 * Initializes the cells, based on the given settings
 * @param x Numbers of cells on the X-coordinate.
 * @param y Numbers of cells on the Y-coordinate.
 * @param bombs The numbers of bombs.
 * @return true, ha sikeres volt a mentés.
 */
/*
Cell* setup(int x, int y, int bombs){
    Cell cells** = (Cell**) malloc(y*sizeof(Cell*));
    cells[0] =
    for(int i=0; i<x; i++){
        for(int j=0; j<y; j++){
            cells[i][j].type = simple;
            cells[i][j].shown = false;
            cells[i][j].marked = false;
        }
    }
    cells = set_bombs(x, y, bombs, cells);
    return cells;
}*/
