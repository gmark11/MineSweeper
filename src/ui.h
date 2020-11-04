#ifndef UI_H
#define UI_H

#include "game.h"

typedef enum Click
{
    right,
    left
} Click;

int open_window();
void click(Cell **c, int x, int y, Click mouse);

#endif
