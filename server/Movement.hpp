//
//  Movement.hpp
//  
//
//  Created by Kuan-Ping Chang  on 1/27/17.
//
//

#ifndef Movement_hpp
#define Movement_hpp

#include <string>

enum class Movement
{
    UP = 0;
    DOWN,
    LEFT,
    RIGHT
};

//for converting direction to integer value
int dirToInt(Movement direction);
//for converting string instruction to direction 
Movement strToDir(std::string dirString);


#endif /* Movement_hpp */
