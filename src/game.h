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
    int x;
    int y;
    GameMode mode;
    Field field;
} Game;

int save(Game game, Cell** cells);
Game new_game(GameMode mode, Field field);
int load_game();
Cell** set_bombs(Game game, int bomb_num, Cell **cells);
Cell** setup(Game game, int bombs);
int show(Cell **c, int x, int y);
void mark(Cell **c, Game game);
void game_over();

#endif
