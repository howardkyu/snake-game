#ifndef MOVE_H
#define MOVE_H

#include <iostream>
#include <string>

enum class Move {
	UP = 0,
	DOWN,
	LEFT,
	RIGHT
};

int moveToInt(Move move);
Move charToMove(std::string moveString);

#endif
