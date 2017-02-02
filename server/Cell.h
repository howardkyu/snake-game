#ifndef CELL_H
#define CELL_H

#include <iostream>

enum class Cell {
	Empty = 0,
	Snake1,
	Snake2,
	Food
};

int cellToInt(Cell cell);

#endif