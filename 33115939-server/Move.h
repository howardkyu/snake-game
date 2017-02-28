// Kuan Hao Yu     33115939
// Kimberly Nguy   86242309
// Pafoua Fang     84351619
// Kuanping Change 71018021

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
