#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <sstream>
#include <random>
#include <ctime>
#include <vector>
#include <queue>
#include <chrono>
#include "websocket.h"
#include <map>
#include "GameBoard.h"

//uses a mapping to keep track of player location
/*  */

using namespace std;
using namespace chrono;

webSocket server;
GameBoard game;
map<int, Snake*> playerMap = map <int, Snake*>();
bool gameOver = true;
const int FPS = 20;
const int MAX_DELAY = 200;
double MS_PER_FRAME = (double)1000000 / FPS;

//default_random_engine is a random number engine class
default_random_engine engine;
string playerOneColor = "red";
string playerTwoColor = "green";
string playerOneDirection = "R";
string playerTwoDirection = "L";
string foodColor = "blue";


list<pair<string, long long>>in_queue = list<pair<string, long long>>();
list<pair<string, long long>>out_queue = list<pair<string, long long>>();
int bufferSize = 100;


//for splitting messages into vector of strings
vector<string> split(string message, char delimiter) {
	vector<string> result = vector<string>();
	stringstream stream(message);
	string token;
	while (getline(stream, token, delimiter)) {
		result.push_back(token);
	}
	return result;
}

//Add delay to the
long long addDelayToTime(string time, int delay)
{
    long long millisec = stoll(time);
    return millisec + delay;
}


//a new send for delaying messages
void delaySend(int clientID, string message)
{
        //convert to get the long long type time for the duration, time_since_epoch returns a duration representing the amount of time bettwen this time and clock's epoch
        long long currentTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        //uniform distribution
        uniform_int_distribution<int> distribution (0, MAX_DELAY);
        //make distribution base on random engine value
        int delay = distribution(engine);
        //if out queue size is smaller than buffersize set the time stamp and push it
        if (out_queue.size() <= bufferSize)
        {
            long long sendTimeStamp = currentTime + delay;
            //client id added to queue for differentiation
            //clientID:message:string sendTimeStamp, sendTimeStamp
            out_queue.push_back(make_pair(to_string(clientID) + ":" + message + ":" + to_string(sendTimeStamp), sendTimeStamp));
        }
}


//send state just updates the client side given them the necessary info for any event changes
string stateString(const vector<pair<Snake::ID, Point>> &changedPositions)
{
	ostringstream os;
	os << "STATE";
	string messageToSend = "STATE";
	for (unsigned int i = 0; i < changedPositions.size(); i++) {
		string changePosition;
		switch (changedPositions[i].first) {
		case Snake::ID::EMPTY:
			changePosition = "EMPTY";
			break; //this is for removing the tail
		case Snake::ID::FOOD:
			changePosition = "FOOD";
			break; //this is for setting/resetting food
		case Snake::ID::PLAYER1:
			changePosition = "PLAYER1";
			break; //this is event for player 1 head position
		case Snake::ID::PLAYER2:
			changePosition = "PLAYER2";
			break; //this is event for player 2 head position
		}
		//this is for storing the coordinates with the type tag 
		os << ":" << changePosition << "," << changedPositions[i].second.x << "," << changedPositions[i].second.y;
	}
	os << ":SCORE1," << game.player1.score << ":SCORE2," << game.player2.score;

	return os.str();

}

/* called when a client connects */
//when the client connects add the player ID in to the game and close the server if more trying to join
void openHandler(int clientID) {

	//Check to see if there are already 2 players connected to the server
	if(playerMap.size() == 2) {
		// Reject the new player
		std::cout << "Rejecting Client: " << clientID << std::endl; // for server debug
		server.wsClose(clientID);
	}

	else {
		std::cout << "Welcome: " << clientID << std::endl; // for server debug
	}
}


/* called when a client disconnects */
//when the client close we want to remove the client ID
void closeHandler(int clientID) {
	std::cout << clientID << " Disconnected" << std::endl;

	if(playerMap.count(clientID) == 1){
		std::cout << "abcd" << std::endl;
		playerMap.erase(clientID);
		if (playerMap.size() < 2) {
			gameOver = true;
			game = GameBoard();
		}		
	}
}



/* called when a client sends a message to the server */
void messageHandler(int clientID, string message) {
	cout << "Receiving raw: " << message << endl;
	vector<string> messageVector = split(message, ':');
    vector<int> clientIDs = server.getClientIDs();

	if (messageVector[0] == "INIT")
	{
		if (playerMap.empty())	/* Add player 1 */
		{
			playerMap[clientID] = &game.player1;
			playerMap[clientID]->name = messageVector[1];
		}
		else if (playerMap.size() == 1)		/* Add player 2 */
		{
			playerMap[clientID] = &game.player2;
			playerMap[clientID]->name = messageVector[1];
			gameOver = false;	// Start the game
		}
		else
		{
		//end
		// std::cout << "Ends: " << clientID << " Client" << std::endl;
			server.wsClose(clientID);
		}

		// Check that there are exactly 2 clients available to play
		if(clientIDs.size() == 2) 
		{
			// Send setup message to clients
			for (unsigned int i = 0; i < clientIDs.size(); i++)
			{
				std::cout << "Sending setup to client " << clientIDs[i] << std::endl;

				//Send the grid size and the color of the food for the player connecting
				string setupMessage = "SETUP:" + to_string(game.grid.size()) + ":" + to_string(game.grid[0].size()) +
					":" + foodColor + ":";
				// Also send the player's snake color
				if(i == 0) {
					setupMessage += "PLAYER1:" + playerOneColor + ":" + playerTwoColor +  ":" + playerMap[1]->name + ":" + playerOneDirection;
				}
				else {
					setupMessage += "PLAYER2:" + playerTwoColor + ":" + playerOneColor +  ":" + playerMap[0]->name + ":" + playerTwoDirection;
				}
				// Send the player the setup info
				
				//original send
				//server.wsSend(clientIDs[i],setupMessage);
				delaySend(clientIDs[i], setupMessage); //sending delayed message
			}
		}
	}
	else 
	{
		// Push the incoming message to the queue
		//incomingMessageBuffer.push(message);
		
		
		uniform_int_distribution<int> distribution{0, MAX_DELAY};
		int delay = distribution(engine);
		if(in_queue.size() <= bufferSize)
		{
			//push back the pair (message, delaytime)
			//add client id for identity
			// std::cout << message;
			in_queue.push_back(make_pair(to_string(clientID) + ":" + message, addDelayToTime(messageVector[messageVector.size() - 1], delay)));
		}
	}
}


void handleMessage(string message){
	cout << "Receiving through queue: " << message << endl;
    // std::cout << clientID << "Enter message handling" << std::endl;
    vector<string> messageVector = split(message, ':');
    int clientID = stoi(messageVector[0]); //from delay the 0 is the clientID

	vector<int> clientIDs = server.getClientIDs();

	//from client message -> "Move, Command"
	if (messageVector[1] == "MOVE" && playerMap[stoi(messageVector[2])]->canMove)
	{
		
		//std::cout << "Server side received: " << messageVector << std::endl;
		//string Move = messageVector[1];
		// std::cout << "Client Move" << messageVector[1] << std::endl;
		string Move = messageVector[3];
		if (Move == "U" && playerMap[clientID]->direction != Move::UP && playerMap[clientID]->direction != Move::DOWN)
		{
			playerMap[clientID]->direction = Move::UP;
			playerMap[clientID]->canMove = false;
		}
		if (Move == "D" && playerMap[clientID]->direction != Move::DOWN && playerMap[clientID]->direction != Move::UP)
		{
			playerMap[clientID]->direction = Move::DOWN;
			playerMap[clientID]->canMove = false;
		}
		if (Move == "L" && playerMap[clientID]->direction != Move::LEFT && playerMap[clientID]->direction != Move::RIGHT)
		{
			playerMap[clientID]->direction = Move::LEFT;
			playerMap[clientID]->canMove = false;
		}
		if (Move == "R" && playerMap[clientID]->direction != Move::RIGHT && playerMap[clientID]->direction != Move::LEFT)
		{
			playerMap[clientID]->direction = Move::RIGHT;
			playerMap[clientID]->canMove = false;
		}
	}

    
    else if (messageVector[1] == "NTP")
    {
        delaySend(clientID, "NTP:" + to_string(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()));
    }
}

/* called once per select() loop */
void periodicHandler(){
	//std::cout << "Enter periodicHandler" << std::endl;
	if (!gameOver){
        chrono::milliseconds start = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        
        if (!in_queue.empty()){
			// cout<< "In Queue not empty" << endl;
            for(int i = 0; i < in_queue.size();i++)
            {
                if(start.count() >= in_queue.front().second)
                {
                    handleMessage(in_queue.front().first);
                    //this checks in queue 
                    in_queue.pop_front();
                
                }
                else
                {
                    break;
                }
            }
        }
        if (!out_queue.empty())
        {
			// cout << "Out queue not empty" << endl;
            for (int i = 0; i < out_queue.size(); i++)
            {
                if(start.count() >= out_queue.front().second)
                {
                    int clientID = stoi(out_queue.front().first.substr(0,1));
                    string message = out_queue.front().first.substr(2);
                    //this is the actual send
                    server.wsSend(clientID, message);
                    out_queue.pop_front();
                     
                }
                else
                {
                	break;
                }
            
            }
        
        }
              
		vector<pair<Snake::ID, Point>> changedCells = game.Update();
		
		string sendString;
		if (!game.isOver){
			sendString = stateString(changedCells);
		}
		else{
			game = GameBoard();
			sendString = "NEWGAME";
		}

		vector<int> clientIDs = server.getClientIDs();
		for (unsigned int i = 0; i < clientIDs.size(); i++){


			// push message to the queue
			//outgoingMessageBuffer.push(clientIDs[i], sendString);
			
			// cout << sendString << endl;
			//server.wsSend(clientIDs[i], sendString);
            delaySend(clientIDs[i], sendString);
        }

		//game.DrawBoard();
		double msDelay = start.count() + MS_PER_FRAME - duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		if (msDelay < 0){
			msDelay = 0;
		}
		usleep(msDelay);
	}
}

int main(int argc, char *argv[]) {

	srand(time(NULL));
	game = GameBoard();
	engine = default_random_engine();
	//init();

	int port;

	cout << "Please set server port: ";
	cin >> port;

	/* set event handler */
	//check at start
	server.setOpenHandler(openHandler);
	//check for end
	server.setCloseHandler(closeHandler);
	//check for messages
	server.setMessageHandler(messageHandler);
	server.setPeriodicHandler(periodicHandler);

	/* start the chatroom server, listen to ip '127.0.0.1' and port '8000' */
	server.startServer(port);

	return 1;
}
