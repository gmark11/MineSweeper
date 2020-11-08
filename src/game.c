#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
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
void mark(Cell **c, Game game)
{
    c[game.x][game.y].marked = true;
}



/**
 * Saves current status to a txt file.
 * @param pj A mentendő játék.
 * @param fajlnev A fájl neve, amit létrehoz.
 * @return 0, if successful save
 */
int save(Game game, Cell** cells)
{
    FILE *fp = fopen("save.txt", "wt");
    fprintf(fp, "MineSweeper Save\n");
    fprintf(fp, "Game settings\n");
    fprintf(fp, "%d %d\n", game.mode, game.field);
    fprintf(fp, "Cells settings\n");
    for(int i=0; i<game.x; i++){
        for(int j=0; j<game.y; j++){
            fprintf(fp, "%d %d %d\n", cells[i][j].type, cells[i][j].shown, cells[i][j].marked);
        }
    }
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
 * Elmenti a játékot egy fájlba.
 * @param pj A mentendő játék.
 * @param fajlnev A fájl neve, amit létrehoz.
 * @return true, ha sikeres volt a mentés.
 */
Game new_game(GameMode mode, Field field)
{
    Game game;
    game.mode = mode;
    game.field = field;

    game.x = game.field;
    game.y = game.field;

    return game;
}


static Cell** set_bombs(Game game, Cell **cells){
    srand(time(NULL));

    int bomb_cells[game.mode][2];

    //TODO: Do not let it generate more than once the same num!!
    for(int i=0; i<game.mode; i++){
        bomb_cells[i][0] = rand() % game.x;
        bomb_cells[i][1] = rand() % game.y;
    }

    for(int i=0; i<game.mode; i++){
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
void setup_cells(Game game, Cell** cells){
    cells = (Cell**) malloc(game.y*sizeof(Cell*));
    for(int x=0; x<game.y; x++){
        cells[x] = (Cell*) malloc(x * sizeof(Cell));
    }

    for(int i=0; i<game.x; i++){
        for(int j=0; j<game.y; j++){
            cells[i][j].type = simple;
            cells[i][j].shown = false;
            cells[i][j].marked = false;
        }
    }


    cells = set_bombs(game, cells);
}
