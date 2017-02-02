#include "Snake.h"
#include <sstream>
#include <list>

	Snake::Snake(ID id, int x, int y, Move direction) {
		this->direction = direction;
		this->score = 0;
		this->id = id;
		this->Add_Point(Point{ x, y });
	}

	int Snake::identity(ID id) {
		switch (id)
		{
		case Snake::ID::EMPTY:
			return 0;
			break;

		case Snake::ID::FOOD:
			return 1;
			break;
		case Snake::ID::PLAYER1:
			return 2;
			break;
		case Snake::ID::PLAYER2:
			return 3;
			break;

		default:
			break;
		}
	}

	void Snake::Add_Point(Point pt)
	{
		this->snake_body.push_front(pt);
		this->head = pt;
	}

	//for animating tail movement
	Point Snake::remove()
	{
		Point tail = this->snake_body.back();
		this->snake_body.pop_back();
		return tail;
	}

	Point Snake::nextLocation(int COLS, int ROWS)
	{
		//head x and y
		int hx = this->head.x;
		int hy = this->head.y;
		switch (this->direction)
		{
		case Move::UP: hy--; break;
		case Move::DOWN: hy++; break;
		case Move::LEFT: hx--; break;
		case Move::RIGHT: hx++; break;
		default: break;
		}
		//negative direction x
		if (hx < 0)
			hx = COLS - 1;
		//positive direction x
		else if (hx > COLS - 1)
			hx = 0;
		//negative direction y
		if (hy < 0)
			hy = ROWS - 1;
		//positive direction y
		else if (hy > ROWS - 1)
			hy = 0;
		return Point{ hx, hy};
	}

	std::string Snake::body()
	{
		std::ostringstream os;
		os << identity(this->id) << "-" << moveToInt(this->direction);
		for (std::list<Point>::iterator it = this->snake_body.begin(); it != this->snake_body.end(); it++) {
			os << "-" << it->x << "," << it->y;
		}
		return os.str();
	}






