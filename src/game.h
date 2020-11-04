#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

typedef enum CellType{
    simple, one, two, three, four, five, bomb
}CellType;

typedef struct Cell{
	CellType type;
	bool shown;
	bool marked;
} Cell;

typedef enum GameMode{
    easy = 10, medium = 20, hard = 30
}GameMode;

typedef enum Field{
    small = 7, medium = 15, big = 30
}Field;

typedef struct Game{
    GameMode mode;
    Field field;
}Game;

typedef enum Click{
    right, left
}Click;

void click(Cell *c, Click mouse);
int save();
int load_game();
int new_game();
int setup(int x, int y, int bombs);

#endif

