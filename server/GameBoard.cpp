//
//  GameBoard.cpp
//  
//
//  Created by Kuan-Ping Chang  on 1/27/17.
//
//

#include "Board.h"


#include <iostream>

using namespace std;


//initialize the game
GameBoard::GameBoard() {

	player1 = Snake(Snake::ID::PLAYER1, COLS / 4, ROWS / 4, Move::RIGHT);
	player2 = Snake(Snake::ID::PLAYER2, COLS / 4, ROWS / 4, Move::LEFT);

	for (int x = 0; x < COLS; x++)
	{
		this->grid.push_back(vector<Snake::ID>());
		for (int y = 0; y < ROWS; y++)
		{
			this->grid[x].push_back(Snake::ID::EMPTY);
		}
	}

	//set snake position
	grid[player1.head.x][player1.head.y] = Snake::ID::PLAYER1;
	grid[player2.head.x][player2.head.y] = Snake::ID::PLAYER2;
	setFood();

}

//for debugging
void GameBoard::DrawBoard() {
	for (int y = 0; y < ROWS; y++)
	{
		for (int x = 0; x < COLS; x++)
		{
			switch (grid[x][y])
			{
			case (Snake::ID::EMPTY):
				cout << " ";
				break;
			case (Snake::ID::PLAYER1):
				cout << "X";
			case (Snake::ID::PLAYER2):
				cout << "O";
				break;
			case (Snake::ID::FOOD):
				cout << "F";
					break;
			default:
				break;

			}
		}
		cout << endl;
	}
	cout << endl;
	cout << "Score: " << player1.score << endl;

}

std::string GameBoard::Update() {
	bool check1 = Move(player1);
		bool check2 = Move(player2);
	if (check1 || check2)
	{
		return "COLLISION";
	}
	else
	{
		return "NULL";
	}
}


void GameBoard::setFood()
{
	vector<Point> legalPts = vector<Point>();
	for (int x = 0; x < COLS; x++) {
		if (this->grid[x][y] == Snake::ID::EMPTY) {
			legalPts.push_back(Point{ x,y });
		}
	}
	int index = rand() % legalPts.size();
	Point randPt = legalPts[index];
	foodPosition = randPt;
	this->grid[randPt.x][randPt.y] = Snake::ID::FOOD;

}

bool GameBoard::Move(Snake& snake)
{
	bool check = false;
	Point nextPt = snake.nextLocation(COLS, ROWS);
	if (grid[nextPt.x][nextPt.y] == Snake::ID::PLAYER1 || grid[nextPt.x][nextPt.y] == Snake::ID::PLAYER2) {
		check = true;
	}
	if (grid[nextPt.x][nextPt.y] == Snake::ID::FOOD)
	{
		snake.score++;
		setFood();
		Point tail = snake.snake_body.back();

	}
	else {
		Point tail = snake.remove();
		grid[tail.x][tail.x] = Snake::ID::EMPTY;
	}
	this->grid[nextPt.x][nextPt.y] = snake.id;
	snake.Add_Point(nextPt);
	return check;

}





