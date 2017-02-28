// Kuan Hao Yu     33115939
// Kimberly Nguy   86242309
// Pafoua Fang     84351619
// Kuanping Change 71018021

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

	Point Snake::nextLocation()
	{
		int nx = this->head.x;
		int ny = this->head.y;

		switch (this->direction)
		{
		case Move::UP: ny--; break;
		case Move::DOWN: ny++; break;
		case Move::LEFT: nx--; break;
		case Move::RIGHT: nx++; break;
		default: break;
		}

		return Point{ nx, ny };
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






