#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <time.h>

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
    Status status;
} Game;

typedef struct Timer{
    clock_t start_time;
    clock_t loaded_time;
    clock_t game_time;
}Timer;

void save(Game *game, Cell ***cells, Timer *timer);
void new_game(Game *game, GameMode mode, Field field);
void check_win(Game *game, int *covered_cells);
void mark(Cell ***cells, int x, int y);
void show(Game *game, Cell ***cells, int x, int y);
void set_status(Status type, Game *game);
void free_memory(Cell **cells, Game *game);
bool load(Game *game, Cell ***cells, Timer *timer);
Cell **setup_cells(Game *game);
Status get_status();
double get_time(Timer *timer);
void set_time(Timer *timer, double t);
void update_time(Timer *timer);

#endif
