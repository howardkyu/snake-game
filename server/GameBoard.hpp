//
//  GameBoard.hpp
//  
//
//  Created by Kuan-Ping Chang  on 1/27/17.
//
//

#ifndef GameBoard_hpp
#define GameBoard_hpp

#include <stdio.h>
#include <vector>
#include "Position.h"
#include "Movement.h"
#include "Snake.h"

// n x n
static int COLS = 10, ROWS = 10;

class GameBoard{
    
public:
    //constructor
    GameBoard();
    
    //Update the current board
    std::string Update();
    
    //Draws everything on the board, mainly for debugging
    void DrawBoard();
    
    //2D vector of grid - n x n dimension
    std::vector<std::vector<Snake::ID>> grid;
    
    Snake player1;
    Snake player2;
    
    //Update snake position and collision
    bool Move(Snake& snake);
    
    
    //Keep track of food location
    Position foodPosition;

private:
    void setFood();

};


#endif /* GameBoard_hpp */
