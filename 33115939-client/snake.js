// Kuan Hao Yu     33115939
// Kimberly Nguy   86242309
// Pafoua Fang     84351619
// Kuanping Change 71018021

var CELL_PX = 10;
var BG_COLOR = "white";
var SNAKE_COLOR;
var OPPONENT_SNAKE_COLOR;
var FOOD_COLOR;


var Server;
var gameCanvas;
var scoreCanvas;
var gameCtx;
var scoreCtx;
var connected;

var playerID;
var playerType; // PLAYER1 OR PLAYER2
var playerDirection;
var playerNumber;
var playerScore;

var opponentID;
var opponentScore;

var clientInitTime;
var clientEndTime;
var ping;

var newGame = true;

function receive(message) {
    console.log("D_RECEIVING: " + message);
    var messageList = message.split(":");

    if (messageList[0] === "ACCEPTED") {
        
        sendWithTime("INIT:" + playerID);

    } else if (messageList[0] === "SETUP") {

        // Parse the message and initialize the variables
        gameSetup(messageList);
    
        init();

        setInterval(pollNTP, 1000); // poll NTP every 1 second

    } else if (messageList[0] === "STATE") {

        for (var i = 1; i < messageList.length; i++) {
            var state = messageList[i].split(",");

            switch(state[0]) {
                case("FOOD"):
                    draw(state[1],state[2], FOOD_COLOR);
                    console.log(FOOD_COLOR);
                    break;
                case("EMPTY"):
                    erase(state[1], state[2]);
                    break;
                case("PLAYER1"):
                    drawPlayerOne(state);
                    break;
                case("PLAYER2"):
                    drawPlayerTwo(state);
                    break;
                case("SCORE1"):
                    updateScore1(state[1]);
                    updateScoreBoard();
                    break;
                case("SCORE2"):
                    updateScore2(state[1]);
                    updateScoreBoard();
                    break;
            }
        }

    } else if (messageList[0] === "NEWGAME") {

        comparePlayersScore();
        displayWinner();
        gameCtx.clearRect(0,0,gameCanvas.width,gameCanvas.height);

    } else if (messageList[0] === "NTP") {

        calculatePing(messageList[1], messageList[2]);
        updatePingText();

    }
}

// Parses the server's message and initializes the variables for the game
function gameSetup(messageList) {

        updateCanvas(Number(messageList[1]), Number(messageList[2]));

        FOOD_COLOR = messageList[3];
        playerType = messageList[4];

        if (playerType == "PLAYER1") {
            playerNumber = 0;
        }
        else {
            playerNumber = 1;
        }
        
        SNAKE_COLOR = messageList[5];
        OPPONENT_SNAKE_COLOR = messageList[6];
        opponentID = messageList[7];
        playerDirection = messageList[8];

}

// Updates the canvas size
function updateCanvas(col, row) {
    gameCanvas.height = col * CELL_PX;
    gameCanvas.width = row * CELL_PX;

    scoreCanvas.width = col / 4 * CELL_PX;
    scoreCanvas.width = row * CELL_PX;
}

// Sets up an event listener to listen for player's keyboard inputs
// Also creates the scoreboard for the game
function init() {
    document.addEventListener('keydown', function(event) {
    
        // Arrow keys
        if (event.keyCode == 38 && playerDirection != 'D') sendDirection('U');
        else if (event.keyCode == 37 && playerDirection != 'R') sendDirection('L');
        else if (event.keyCode == 40 && playerDirection != 'U') sendDirection('D');
        else if (event.keyCode == 39 && playerDirection != 'L') sendDirection('R');
    });

    gameCtx = gameCanvas.getContext("2d");
    scoreCtx = scoreCanvas.getContext("2d");

    updateScoreBoard();

}

// Connects to the server
function connect() {
    Server = new FancyWebSocket('ws://' + document.getElementById('ip').value + ':' + document.getElementById('port').value);
    
    Server.bind('open', function() {
        // document.getElementById('cnt-btn').disabled = true;

        gameCanvas = document.getElementById("canvas-game");
        scoreCanvas = document.getElementById("canvas-score");

        playerID = document.getElementById('player-id').value;

    });

    Server.bind('close', function(data) {
        document.getElementById('cnt-btn').disabled = false;
    });

    Server.bind('message', receive);

    Server.connect();
}

// Updates the player's new direction and also sends the server the direction as well
function sendDirection(direction) {
    playerDirection = direction;
    sendWithTime("MOVE:" + playerNumber + ":" + playerDirection);
}

// Determines which color snake to draw when server sends a message that is addressed to "PLAYER1"
function drawPlayerOne(state) {
    if (playerType == "PLAYER1") {  
        draw(state[1], state[2], SNAKE_COLOR);
    }   
    else {
        draw(state[1], state[2], OPPONENT_SNAKE_COLOR);
    }      
}

// Determines which color snake to draw when server a message that is addressed to "PLAYER2"
function drawPlayerTwo(state) {
    if (playerType == "PLAYER2") {   
        draw(state[1], state[2], SNAKE_COLOR);
    }   
    else {
        draw(state[1], state[2], OPPONENT_SNAKE_COLOR);
    }  
}

// Draws
function draw(x, y, color) {
   gameCtx.fillStyle = color;
   gameCtx.fillRect(x * CELL_PX, y * CELL_PX, CELL_PX, CELL_PX); 
}


// Draws the cell white to erase the color
function erase(x, y) {
   draw(x,y, BG_COLOR);
}


// Determines which score to update when server sends a message that contains "SCORE1"
function updateScore1(state) {
    if (playerType == "PLAYER1") {
        playerScore = state;
    }
    else {
        opponentScore= state;
    }     
}


// Determines which score to update when server sends a message that contains "SCORE2"
function updateScore2(state) {
    if (playerType == "PLAYER2") {
        playerScore = state;
    }
    else {
        opponentScore= state;
    }     
}


function updateScoreBoard() {
    // If not a new game, then clear the canvas to update the score
    if(!newGame) {
        scoreCtx.clearRect(0,0,scoreCanvas.width,scoreCanvas.height);
    }
    else {
        newGame = false;
    }
    
    scoreCtx.font = "30px Ariel";
    scoreCtx.textAlign = "center";
    scoreCtx.fillText(playerID + "'s Score: " + playerScore, scoreCanvas.width*2/8, scoreCanvas.height/2);
    scoreCtx.fillText(opponentID + "'s Score: " + opponentScore, scoreCanvas.width*6/8, scoreCanvas.height/2);   
}


// Compares the players' scores to see who is the winner
function comparePlayersScore() {
    if(playerScore > opponentScore) {
        return playerID.toUpperCase() + " WINS!";
    }
    else if (playerScore < opponentScore) {
        return opponentID.toUpperCase() + " WINS!";
    }
    else {
        return "IT'S A TIE!";
    }
}

// Displays a text stating who won the game
function displayWinner(text) {
    gameCtx.clearRect(0,0,gameCanvas.width,gameCanvas.height);
    gameCtx.font = "40px Ariel";
    gameCtx.fillStyle = "red";
    gameCtx.textAlign = "center";
    var winner = comparePlayersScore();
    gameCtx.fillText("GAME OVER: " + winner, gameCanvas.width/2, gameCanvas.height/2);
}

function pollNTP() {
    sendWithTime("NTP");
}

function calculatePing(initTime, endTime) {
    clientEndTime = new Date().getTime();
    serverInitTime = parseInt(initTime);
    serverEndTime = parseInt(endTime);
    ping = (clientEndTime - clientInitTime) - (serverEndTime - serverInitTime);
}

function updatePingText() {
    var pingDiv = document.getElementById("ping");
    pingDiv.innerHTML = "Ping: " + ping + " ms";
}

function sendWithTime(message) {
    console.log("D_SENDING: " + message);
    clientInitTime = new Date().getTime();
    Server.send("message", message + ":" + clientInitTime);
}
