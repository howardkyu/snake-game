//
//  Snake.cpp
//  
//
//  Created by Kuan-Ping Chang  on 1/27/17.
//
//

#include "Snake.hpp"


    int identity (Snake::ID id){
    switch(id)
    {
        case Snake::ID::EMPTY;
            return 0;
            break;
            
        case Snake::ID::FOOD;
            return 1;
            break;
        case Snake::ID::PLAYER1;
            return 2;
            break;
        case Snake::ID::PLAYER2;
            return 3;
            break;
            
        default:
            break;
    }

    Snake::Snake (Snake::ID id, int x, int y, Movement direction){
        this -> direction = direction;
        this -> score = 0;
        this -> id = id;
        this -> Add(Position{x, y});
    }

    void Snake::Add(Position pt)
    {
        this -> snake_body.push_front(pt);
        this -> head = pt;
    }
    
    //for animating tail movement
    Position Snake::remove()
    {
        Position tail = this -> queue.back();
        this -> snake_body.pop_back();
        return tail;
    }

    Point Snake::nextLocation(int COLS, int ROWS)
    {
        //head x and y
        int hx = this -> head.x;
        int hy = this -> head.y;
        switch(this -> direction)
        {
            case Movement::UP : hy--; break;
            case Movement::DOWN : hy++; break;
            case Movement::LEFT : hx--; break;
            case Movement::RIGHT : hx++; break;
            default: break;
        }
        //negative direction x
        if (hx < 0)
            hx = COLS -1;
        //positive direction x
        else if (hx > COLS - 1)
            hx = 0;
        //negative direction y
        if (hy < 0)
            hy = ROWS -1;
        //positive direction y
        else if (hy > ROWS - 1)
            hy = 0;
        return Position{hx, hy};
    }
        
    std::string Snake::body()
    {
        ostringstream os;
        os << identity(this->Snake::ID) << "-" << dirToInt(this->direction);
        for (list<Position>::iterator it = this->snake_body.begin(); it != this->snake_body.end(); it++){
                os << "-" << it -> x << "," << it -> y;
        }
        return os.str();
    }
        
        
        
        
        
