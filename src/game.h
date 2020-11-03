#ifndef GAME_H
#define GAME_H

typedef enum CellType{
    simple, one, two, free, four, five, bomb
}CellType;

typedef struct Cell{
	CellType type;
	bool shown;
	bool marked;
} Cell;

typedef struct Game{

}

typedef enum Click{
    right, left
}Click;

#endif

