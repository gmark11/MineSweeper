#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

typedef enum CellType
{
    simple,
    one,
    two,
    three,
    four,
    five,
    bomb
} CellType;

typedef struct Cell
{
    CellType type;
    bool shown;
    bool marked;
} Cell;

typedef enum GameMode
{
    easy_mode = 10,
    medium_mode = 20,
    hard_mode = 30
} GameMode;

typedef enum Field
{
    small_field = 7,
    medium_field = 15,
    big_field = 30
} Field;

typedef struct Game
{
    GameMode mode;
    Field field;
} Game;

void save(Game *game, Cell **cells);
void new_game(Game *game, GameMode mode, Field field);
void mark(Cell ***c, int x, int y);
void show(Cell ***c, int x, int y);
Cell **setup_cells(Game *game);

#endif
