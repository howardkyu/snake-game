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
#include <chrono>
#include "websocket.h"
#include <map>
#include "GameBoard.h"

//uses a mapping to keep track of player location
/*  */

using namespace std;
using namespace std::chrono;

webSocket server;
GameBoard game;
map<int, Snake*> playerMap = map <int, Snake*>();
bool gameOver = true;
const int FPS = 20;
double MS_PER_FRAME = (double)1000000 / FPS;

string playerOneColor = "red";
string playerTwoColor = "green";
string playerOneDirection = "R";
string playerTwoDirection = "L";
string foodColor = "blue";

struct message {
	String playerId;
	String message;
};

queue<message> incomingMessageBuffer;
queue<message> outgoingMessageBuffer;


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

//send state just updates the client side given them the necessary info for any event changes
string stateString(const vector<pair<Snake::ID, Point>> &changedPositions) {
	ostringstream os;
	String messageToSend = "STATE";
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
		messageToSend += ":" + changePosition + "," + changedPositions[i].second.x ++ "," + changedPositions[i].second.y;
	}
	messageToSend = ":SCORE1," + game.player1.score + ":SCORE2," + game.player2.score;
	os << messageToSend;

	return os.str();

}

/* called when a client connects */
//when the client connects add the player ID in to the game and close the server if more trying to join
void openHandler(int clientID) {
	std::cout << "Welcome: " << clientID << std::endl; // for server debug
}

/* called when a client disconnects */
//when the client close we want to remove the client ID
void closeHandler(int clientID) {
	std::cout << clientID << " Disconnected" << std::endl;
	playerMap.erase(clientID);
	if (playerMap.size() < 2) {
		gameOver = true;
		game = GameBoard();
		
	}
}

/* called when a client sends a message to the server */
void messageHandler(int clientID, string message) {
	// Push the incoming message to the queue
	incomingMessageBuffer.push(message);
	// std::cout << clientID << "Enter message handling" << std::endl;
	vector<string> messageVector = split(message, ':');

	cout << message << endl;
	if (messageVector[0] == "INIT")
	{
		if (playerMap.empty()) {
			//add player 1
			// std::cout << "Add: " << messageVector[1] << "as Player1" << std::endl;
			//playerMap[clientID] = &game.player1;
			playerMap[0] = &game.player1;
			playerMap[0]->name = messageVector[1];
		}
		else if (playerMap.size() == 1)
		{
			//add player 2
			// std::cout << "Add: " << messageVector[1] << "as Player2" << std::endl;
			//playerMap[clientID] = &game.player2;
			playerMap[1] = &game.player2;
			playerMap[1]->name = messageVector[1];
			gameOver = false;
		}
		else
		{
			//end 
			// std::cout << "Ends: " << clientID << " Client" << std::endl;
			server.wsClose(clientID);
		}

		// playerMap[clientID]->name = messageVector[1];

		vector<int> clientIDs = server.getClientIDs();

		// Check that there are exactly 2 clients available to play
		if(clientIDs.size() == 2) {
			//get all the id in server
			//send them individual information that needed to be updated
			for (unsigned int i = 0; i < clientIDs.size(); i++)
			{
				std::cout << "Sending Message to: " << messageVector[1] << std::endl;

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

				// Push message to the queue
				outgoingMessageBuffer.push(clientIDs[i], setupMessage);

				// Send the player the setup info
				server.wsSend(clientIDs[i],setupMessage);
			
			}
		}	
	}

	//from client message -> "Move, Command"
	else if (messageVector[0] == "MOVE" && playerMap[stoi(messageVector[1])]->canMove)
	{
		int clientID = stoi(messageVector[1]);
		//std::cout << "Server side received: " << messageVector << std::endl;
		//string Move = messageVector[1];
		// std::cout << "Client Move" << messageVector[1] << std::endl;
		string Move = messageVector[2];
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

	// NTP poll from client
	else if (messageVector[0] == "NTP") {
		milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
		milliseconds ms1 = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

		string send = "NTP:" + to_string(ms.count()) + ":" + to_string(ms2.count());
		server.wsSend(clientIDs[i], send);
	}
}

/* called once per select() loop */
void periodicHandler(){
	//std::cout << "Enter periodicHandler" << std::endl;
	if (!gameOver){
		clock_t start = clock() / (CLOCKS_PER_SEC / 1000000);
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
			outgoingMessageBuffer.push(clientIDs[i], sendString);
			
			// cout << sendString << endl;
			server.wsSend(clientIDs[i], sendString);
		}

		//game.DrawBoard();
		double msDelay = (start / (CLOCKS_PER_SEC / 1000000)) + MS_PER_FRAME - (clock() / (CLOCKS_PER_SEC / 1000000));
		if (msDelay < 0){
			msDelay = 0;
		}
		usleep(msDelay);
	}
}

int main(int argc, char *argv[]) {

	srand(time(NULL));
	game = GameBoard();
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
