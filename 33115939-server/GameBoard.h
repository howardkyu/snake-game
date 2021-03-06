// Kuan Hao Yu     33115939
// Kimberly Nguy   86242309
// Pafoua Fang     84351619
// Kuanping Change 71018021

#ifndef GameBoard_H
#define GameBoard_H

#include <stdio.h>
#include <vector>
#include "Point.h"
#include "Move.h"
#include "Snake.h"

// n x n
static int COLS = 60, ROWS = 60;

class GameBoard {

public:
	//constructor
	GameBoard();

	//Update the current board
	std::vector<std::pair<Snake::ID, Point>> Update();

	//Draws everything on the board, mainly for debugging
	void DrawBoard();

	//2D vector of grid - n x n dimension
	std::vector<std::vector<Snake::ID>> grid;

	Snake player1;
	Snake player2;

	//Keep track of food location
	Point foodPosition;
	bool isOver = false;

private:
	void setFood();
	void Move(Snake& snake);
	std::vector<std::pair<Snake::ID, Point>> changedPositions = std::vector<std::pair<Snake::ID, Point>>();
};


#endif /* GameBoard_hpp */
#pragma once
