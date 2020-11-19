#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

typedef enum Status
{
    ingame,
    win,
    gameover
} Status;

typedef enum CellType
{
    simple,
    one,
    two,
    three,
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

void save(Game *game, Cell ***cells);
void new_game(Game *game, GameMode mode, Field field);
void check_win(Game *game, int *covered_cells);
void mark(Cell ***cells, int x, int y);
void show(Game *game, Cell ***cells, int x, int y);
void set_status(Status type);
bool load(Game *game, Cell ***cells);
Cell **setup_cells(Game *game);
Status get_status();

#endif
