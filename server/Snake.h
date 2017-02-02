
#ifndef Snake_h
#define Snake_h

#include <string>
#include <vector>
#include <list>
#include "Move.h"
#include "Point.h"
#include <string>
#include <list>
#include <vector>

class Snake {

public:

	enum class ID {
		EMPTY = 0,
		FOOD,
		PLAYER1,
		PLAYER2

	};
	int identity(ID id);

	//main snake
	Snake() {};
	Snake(ID id, int x, int y, Move direction);
	void Add_Point(Point pt);
	Point remove();
	Point nextLocation(int COLS, int ROWS);
	std::string body();
	std::string name = "";
	Move direction;
	int score;
	ID id;
	std::list<Point> snake_body;
	Point head;
};


#endif 
