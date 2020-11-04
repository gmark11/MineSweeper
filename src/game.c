#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "game.h"

/**
 * Marks the selected cell.
 * @param c A The clicked cell.
 * @return true, ha sikeres volt a mentés.
 */
void game_over(){}

/**
 * Shows the selected cell.
 * @param c A The clicked cell.
 * @return true, ha sikeres volt a mentés.
 */
int show(Cell **c, int x, int y)
{
    c[x][y].shown = true;
    if (c[x][y].type == bomb)
    {
        game_over();
    }
    else
    {
        switch (c[x][y].type)
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
int mark(Cell **c, int x, int y)
{
    c[x][y].marked = true;
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
    //time
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


Cell** set_bombs(int x, int y, int bomb_num, Cell **cells){
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
    return cells;
}


/**
 * Initializes the cells, based on the given settings
 * @param x Numbers of cells on the X-coordinate.
 * @param y Numbers of cells on the Y-coordinate.
 * @param bombs The numbers of bombs.
 * @return true, ha sikeres volt a mentés.
 */
Cell** setup(int x, int y, int bombs){
    Cell **cells = (Cell**) malloc(y*sizeof(Cell*));
    for(x=0; x<y; x++){
        cells[x] = (Cell*) malloc(x * sizeof(Cell));
    }

    for(int i=0; i<x; i++){
        for(int j=0; j<y; j++){
            cells[i][j].type = simple;
            cells[i][j].shown = false;
            cells[i][j].marked = false;
        }
    }
    //cells = set_bombs(x, y, bombs, cells);
    return cells;
}
