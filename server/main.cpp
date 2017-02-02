#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include <vector>
#include "websocket.h"
#include <map>
#include "Board.h"

//uses a mapping to keep track of player location
/*  */

using namespace std;

webSocket server;
GameBoard game;
map<int, Snake*> playerMap = map <int, Snake*>();

void init()
{
	game = GameBoard();
}

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
	std::cout << "Welcome: " << clientID << std::endl;
	if (playerMap.empty()) {
		playerMap[clientID] = &game.player1;
	}
	else if (playerMap.size() == 1)
	{
		playerMap[clientID] = &game.player2;
	}
	else
	{
		//server.wsSend(clientID, "ERROR NEW PLAYER NOT ACCEPTED");
		server.wsClose(clientID);
	}

}

/* called when a client disconnects */
//when the client close we want to remove the client ID
void closeHandler(int clientID) {
	std::cout << clientID << " Disconnected" << std::endl;
	playerMap.erase(clientID);
	if (playerMap.size() < 2) {
		init();
	}
}

/* called when a client sends a message to the server */
void messageHandler(int clientID, string message) {

	handleMessage(clientID, message);

}

void handleMessage(int clientID, string message)
{
	vector<string> messageVector = split(message, ':');
	if (messageVector[0] == "START")
	{
		//from client message -> "Command, PlayerID"
		playerMap[clientID]->name = messageVector[1];
		if (playerMap.size() == 2)
		{
			vector<int> clientIDs = server.getClientIDs();
			//sending the initialization information when the message is START from the client
			//client id -> "START" ...server returns a string of Setups that needed to be split into string by function similar to message Handler
			for (unsigned int i = 0; i < clientIDs.size(); i++)
			{
				ostringstream os;

				os << to_string(game.grid.size()) << ":" << to_string(game.grid[0].size()) << ":" << game.player1.name << ":" << game.player2.name << ":" << to_string(Snake:identity(playerMap[clientIDs[i]]->id)));
				server.wsSend(clientIDs[i], os.str());
				//server.wsSend("Ready");
			}
		}

	}
	//from client message -> "Move, Command"
	else if (messageVector[0] == "MOVE")
	{
		cout << "Player: " << clientID << " Message: " << message << endl;

		string dirString = messageVector[1]; //index depending on message splits and positions

		int moveResult = MoveHandler(clientID, dirString);

		//start new game if any of the move is invalid
		if (moveResult != 0)
		{
			vector<int> clientIDs = server.getClientIDs();
			for (unsigned int i = 0; i < clientIDs.size(); i++)
			{
				server.wsSend(clientIDs[i], "NEWGAME");
			}
			init();
		}
		//game.DrawBoard();
	}
}

int MoveHandler(int clientID, string dirString)
{
	//if dirString = "D" then direction should return Movement::DOWN
	string check = game.Update();
	//if a collision happens it will return -1
	if (check == "COLLISION") {
		cout << "collision" << endl;
		return -1;
	}
	//if direction string is not null then convert it to direction
	if (dirString != "NULL")
	{
		playerMap[clientID]->direction = charToMove(dirString);
	}

	//create a outstream
	ostringstream os;
	//set outstream formated: "TYPE:FOOD POSX, FOOD POSY:P1 body:P2 body:P1 Score:P2 Score
	os << "GAMESTATE:" << game.foodPosition.x << "," << game.foodPosition.y << ":" << game.player1.body() << ":" << game.player2.body() << ":" << game.player1.score << ":" << game.player2.score;

	//find the client id
	vector<int> clientIDs = server.getClientIDs();

	//Send to both client with the current info
	for (unsigned int i = 0; i < clientIDs.size(); i++)
	{
		server.wsSend(clientIDs[i], os.str());
	}

	//if everything is fine then it will return 0
	return 0;
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
		vector<int> clientIDs = server.getClientIDs();
		for (int i = 0; i < clientIDs.size(); i++)
		{
			server.wsSend(clientIDs[i], os.str());
		}
		next = time(NULL) + 10;
	}
}

int main(int argc, char *argv[]) {

	srand(time(NULL));
	init();

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
	//server.startServer(port);

	//return 1;
	return 0;
}
