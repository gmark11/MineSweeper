#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "game.h"

Cell cells[][];

/**
 * Handles mouse clicks (left or right).
 * @param c The clicked cell.
 * @param mouse The type of the mouse click (left or right).
 * @return void
 */
void click(Cell *c, Click mouse){
    if(mouse==left){
        show(c);
    }
    if(mouse==right){
        mark(c);
    }
}

/**
 * Elmenti a játékot egy fájlba.
 * @param pj A mentendő játék.
 * @param fajlnev A fájl neve, amit létrehoz.
 * @return true, ha sikeres volt a mentés.
 */
int save(){

}

/**
 * Elmenti a játékot egy fájlba.
 * @param pj A mentendő játék.
 * @param fajlnev A fájl neve, amit létrehoz.
 * @return true, ha sikeres volt a mentés.
 */
int load_game(){

}

/**
 * Elmenti a játékot egy fájlba.
 * @param pj A mentendő játék.
 * @param fajlnev A fájl neve, amit létrehoz.
 * @return true, ha sikeres volt a mentés.
 */
int new_game(){

}

/**
 * Initializes the cells, based on the given settings
 * @param x Numbers of cells on the X-coordinate.
 * @param y Numbers of cells on the Y-coordinate.
 * @param bombs The numbers of bombs.
 * @return true, ha sikeres volt a mentés.
 */
int setup(int x, int y, int bombs){
    for(int i=0; i<x; i++){
        for(int j=0; j<y; j++){
            //cells[i][j].type = ;
            cells[i][j].shown = false;
            cells[i][j].marked = false;
        }
    }
}

/**
 * Shows the selected cell.
 * @param c A The clicked cell.
 * @return true, ha sikeres volt a mentés.
 */
static int show(Cell *c){
    c->shown = true;
    if(c->type==bomb){
        game_over();
    }
    else{
        switch (c->type) {
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
}

/**
 * Marks the selected cell.
 * @param c A The clicked cell.
 * @return true, ha sikeres volt a mentés.
 */
static int mark(Cell *c){
    c->marked = true;
}

/**
 * Marks the selected cell.
 * @param c A The clicked cell.
 * @return true, ha sikeres volt a mentés.
 */
static void game_over(){

}