//
//  GameBoard.cpp
//  
//
//  Created by Kuan-Ping Chang  on 1/27/17.
//
//

#include "GameBoard.hpp"


#include <iostream>

using namespace std;


//initialize the game
GameBoard::GameBoard(){
    
    player1 = Snake(Snake::ID::PLAYER1, COLS/4, ROWs/4, Movement::RIGHT);
    player2 = Snake(Snake::ID::PLAYER2, COLS/4, ROWS/4, Movement::LEFT);

    for (int x = 0; x < COLS; x++)
    {
        this -> grid.push_back(vector<ID>());
        for (int y = 0; y < ROWS; y++)
        {
            this ->grid[x].push_back(ID::EMPTY);
        }
    }

    //set snake position
    grid[player1.head.x][player1.head.y] = ID::PLAYER1;
    grid[player2.head.x][player2.head.y] = ID::PLAYER2;
    
    
}

//for debugging
GameBoard::DrawBoard(){
    for (int y = 0; y < ROWS; y++)
    {
        for (int x = 0; x < COLS; x++)
        {
            switch(grid[x][y])
            {
                case (ID::EMPTY):
                    cout << " ";
                    break;
                case (ID::PLAYER1):
                    cout << "X";
                case (ID::PLAYER2):
                    cout << "O";
                    break;
                case (ID::FOOD):
                    cout << "F"l
                    break;
                default:
                    break;
            
            }
        }
        cout << endl;
    }
    cout << endl;
    cout << "Score: " << snake1.score << endl;



}
