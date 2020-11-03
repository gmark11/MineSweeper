#include <stdio.h>
#include <stdlib.h>
#include "game.h"

/**
 * Elmenti a játékot egy fájlba.
 * @param pj A mentendő játék.
 * @param fajlnev A fájl neve, amit létrehoz.
 * @return true, ha sikeres volt a mentés.
 */
int generate_map(){

}

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
 * Shows the selected cell.
 * @param c A The clicked cell.
 * @return true, ha sikeres volt a mentés.
 */
static int show(Cell *c){
    c->checked = true;
}

/**
 * Marks the selected cell.
 * @param c A The clicked cell.
 * @return true, ha sikeres volt a mentés.
 */
static int mark(Cell *c){
    c->marked = true;
}
