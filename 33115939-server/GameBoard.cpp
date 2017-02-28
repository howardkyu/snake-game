// Kuan Hao Yu     33115939
// Kimberly Nguy   86242309
// Pafoua Fang     84351619
// Kuanping Change 71018021


#include <iostream>
#include "GameBoard.h"

using namespace std;


//initialize the game
GameBoard::GameBoard() {

	player1 = Snake(Snake::ID::PLAYER1, COLS / 4, ROWS / 2, Move::RIGHT);
	player2 = Snake(Snake::ID::PLAYER2, COLS - (COLS / 4), ROWS / 2, Move::LEFT);

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
	system("cls");
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

std::vector<std::pair<Snake::ID, Point>> GameBoard::Update(){
	changedPositions.clear();
	changedPositions.push_back(make_pair(Snake::ID::FOOD, foodPosition));
	Move(player1);
	Move(player2);
	return changedPositions;
}


void GameBoard::setFood()
{
	vector<Point> legalPts = vector<Point>();
	for (int x = 0; x < COLS; x++) {
		for (int y = 0; y < ROWS; y++) {

			if (this->grid[x][y] == Snake::ID::EMPTY) {
				legalPts.push_back(Point{ x,y });
			}
		}
	}
	int index = rand() % legalPts.size();
	Point randPt = legalPts[index];
	foodPosition = randPt;
	this->grid[randPt.x][randPt.y] = Snake::ID::FOOD;

}

void GameBoard::Move(Snake& snake)
{
	Point nextPoint = snake.nextLocation();

	if (nextPoint.x < 0)
		nextPoint.x = COLS - 1;
	else if (nextPoint.x > COLS - 1)
		nextPoint.x = 0;
	if (nextPoint.y < 0)
		nextPoint.y = ROWS - 1;
	else if (nextPoint.y > ROWS - 1)
		nextPoint.y = 0;

	//check collision
	if (grid[nextPoint.x][nextPoint.y] == Snake::ID::PLAYER1 || grid[nextPoint.x][nextPoint.y] == Snake::ID::PLAYER2) {
		std::cout << "COLLIDED" << std::endl;
		isOver = true;
	}

	changedPositions.push_back(make_pair(snake.id, nextPoint));	//for head rendering

	if (grid[nextPoint.x][nextPoint.y] == Snake::ID::FOOD) {
		snake.score++;
		setFood();
		Point tail = snake.snake_body.back();
		changedPositions.push_back(make_pair(snake.id, tail));	//for tail rendering
	}
	else {
		Point tail = snake.remove();
		grid[tail.x][tail.y] = Snake::ID::EMPTY;
		changedPositions.push_back(make_pair(Snake::ID::EMPTY, tail));	//for empty position
	}
	this->grid[nextPoint.x][nextPoint.y] = snake.id;
	snake.Add_Point(nextPoint);
	snake.canMove = true;

}





