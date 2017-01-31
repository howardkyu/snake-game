//
//  Movement.cpp
//  
//
//  Created by Kuan-Ping Chang  on 1/27/17.
//
//

#include "Movement.hpp"

int dirToInt(Movement direction)
{
    switch(direction)
    {
        case Movement::UP;
            return 0;
            break;
            
        case Movement::DOWN;
            return 1;
            break;
            
        case Movement::LEFT;
            return 2;
            break;
            
        case Movement::RIGHT;
            return 3;
            break;
        default:
            break;
    }
}


Movement strToDir(std::string dirString)
{
    if (dirString == "UP")
    {
        return Movement::UP;
    }
    else if (dirString == "DOWN")
    {
        return Movement::DOWN;
    }
    else if (dirString == "LEFT")
    {
        return Movement::LEFT;
    }
    else if (dirString == "RIGHT")
    {
        return Movement::RIGHT;
    }
}
