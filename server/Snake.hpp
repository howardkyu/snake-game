//
//  Snake.hpp
//  
//
//  Created by Kuan-Ping Chang  on 1/27/17.
//
//

#ifndef Snake_hpp
#define Snake_hpp

#include <string>
#include <vector>
#include <list>
#include "Movement.h"
#include "Position.h"

class Snake{
    
public:
    
    enum class ID{
        EMPTY = 0,
        FOOD,
        PLAYER1,
        PLAYER2
        
    };
    int identity(ID id);
    
    //main snake
    Snake(){};
    Snake(ID id, int x, int y, Movement direction);
    void Add (Position pt);
    Position remove();
    Position nextLocation(int COLS, int ROWS);
    std::string body();
    std::string name = "";
    Movement direction;
    int score;
    ID id;
    std::list<Position> snake_body;
    Position head;
};


#endif /* Snake_hpp */
