#include "Move.h"
#include <string>

int moveToInt(Move move) {
	switch (move) {
		case Move::UP:
			return 0;
			break;
		case Move::DOWN:
			return 1;
			break;
		case Move::LEFT:
			return 2;
			break;
		case Move::RIGHT:
			return 3;
			break;
		default: 
			break;
	}
}

Move charToMove(std::string moveString) {
	if (moveString == "U") {
		return Move::UP;
	}
	if (moveString == "D") {
		return Move::DOWN;
	}
	if (moveString == "L"){
		return Move::LEFT;
	}
	if (moveString == "R") {
		return Move::RIGHT;
	}

}