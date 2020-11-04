#include <stdlib.h>
#include <stdio.h>
#include "ui.h"
#include "game.h"

int main()
{
    open_window();
    Cell** cells = setup(5, 5, 2);

    //free(cells);
    return 0;
}
