#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include <vector>
#include "websocket.h"
#include <map>
#include "GameBoard.h"

//uses a mapping to keep track of player location
/*  */

using namespace std;

webSocket server;
GameBoard game;
map<int, Snake*> playerMap = map <int, Snake*>();

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

/* called when a client connects */
//when the client connects add the player ID in to the game and close the server if more trying to join
void openHandler(int clientID) {
	std::cout << "Welcome: " << clientID << std::endl; // for server debug
	if (playerMap.empty()) {
		//add player 1
		playerMap[clientID] = &game.player1;
	}
	else if (playerMap.size() == 1)
	{
		//add player 2
		playerMap[clientID] = &game.player2;
	}
	else
	{
		//end 
		server.wsClose(clientID);
	}
	//send the grid size for the player coonnecting
	server.wsSend(clientID, "SETUP:" + to_string(game.grid.size()) + ":" + to_string(game.grid[0].size()));
}

/* called when a client disconnects */
//when the client close we want to remove the client ID
void closeHandler(int clientID) {
	std::cout << clientID << " Disconnected" << std::endl;
	playerMap.erase(clientID);
	if (playerMap.size() < 2) {
		game = Game();
	}
}

/* called when a client sends a message to the server */
void messageHandler(int clientID, string message) {

	vector<string> messageVector = split(message, ':');
	//from client message -> "Move, Command"
	if (messageVector[0] == "MOVE" && playerMap[clientID]->canMove)
	{
		string direction = messageVector[1];
		if (direction == "UP" && playerMap[clientID]->direction != Direction::UP && playerMap[clientID]->direction != Direction::DOWN) 
		{
			playerMap[clientID]->direction = Direction::UP;
			playerMap[clientID]->canMove = false;
		}
		if (direction == "DOWN" && playerMap[clientID]->direction != Direction::DOWN && playerMap[clientID]->direction != Direction::UP) 
		{
			playerMap[clientID]->direction = Direction::DOWN;
			playerMap[clientID]->canMove = false;
		}
		if (direction == "LEFT" && playerMap[clientID]->direction != Direction::LEFT && playerMap[clientID]->direction != Direction::RIGHT) 
		{
			playerMap[clientID]->direction = Direction::LEFT;
			playerMap[clientID]->canMove = false;
		}
		if (direction == "RIGHT" && playerMap[clientID]->direction != Direction::RIGHT && playerMap[clientID]->direction != Direction::LEFT)
		{
			playerMap[clientID]->direction = Direction::RIGHT;
			playerMap[clientID]->canMove = false;
		}
	}
}

/* called once per select() loop */
void periodicHandler() {
	static time_t next = time(NULL) + 10;
	time_t current = time(NULL);
	if (current >= next)
	{
		ostringstream os;
		string timestring = ctime(&current);
		timestring = timestring.substr(0, timestring.size() - 1);
		os << timestring;

		//get all the id and sends the result to client 
		//also the updates 
		vector<pair<Snake::ID, Point>> changePositions = game.Update();
		//sending the message
		sendState(changedPositions);
		next = time(NULL) + 10;
	}
}

//send state just updates the client side given them the necessary info for any event changes
void sendState(const vector<pair<Cell, Point>> &changedPositions) {
	ostringstream os;
	os << "DRAW";
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
		os << ":" << changePosition << ":" << changedPositions[i].second.x << ":" << changedPositions[i].second.y;

		///sample implementation of bind in client 
		/*
		Server.bind('message', function(message){
        var messageList = message.split(':');
        if (messageList[0] === "SETUP"){
        	COLS = parseInt(messageList[1]);
        	ROWS = parseInt(messageList[2]);
        	main();
        }
        if (messageList[0] === "DRAW"){	//messageList will look like DRAW:CELL,X,Y:CELL,X,Y:CELL,X,Y:CELL,X,Y:CELL,X,Y (5 cells & their locations)        	
        	for (var index = 1; index <= 5; index++){
        		var cellInfo = messageList[index].split(',');
        		var cellID = cellInfo[0], x = cellInfo[1], y = cellInfo[2];
        		if (cellID === "EMPTY")
        			grid[x][y] = EMPTY;
        		else if (cellID === "FOOD")
        			grid[x][y] = FOOD;
        		else if (cellID === "SNAKE1")
        			grid[x][y] = SNAKE;
        		else if (cellID === "SNAKE2")
        				grid[x][y] = SNOKE;
        		}
			}
		 });
		*/
		
		
	}

	//os should be in the format of DRAW:TYPE:X:Y	
	//the complete version will have DRAW:EMPTY:x:y:FOOD::x:y:PLAYER1:x:y:PLAYER2:x:y
	
	vector<int> clientIDs = server.getClientIDs();
	for (unsigned int i = 0; i < clientIDs.size(); i++) {
		server.wsSend(clientIDs[i], os.str());
	}
}


int main(int argc, char *argv[]) {

	srand(time(NULL));
	game = Game();
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
