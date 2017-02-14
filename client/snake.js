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

var newGame = true;

function receive(message) {
    var messageList = message.split(":");

    if (messageList[0] === "SETUP") {
        updateCanvas(Number(messageList[1]), Number(messageList[2]));

        FOOD_COLOR = messageList[3];
        playerType = messageList[3];
        if (playerType == "PLAYER1") {
            playerNumber = 0;
        }
        else {
            playerNumber = 1;
        }
        
        SNAKE_COLOR = messageList[5];
        console.log(SNAKE_COLOR);
        OPPONENT_SNAKE_COLOR = messageList[6];
        console.log(OPPONENT_SNAKE_COLOR);
        opponentID = messageList[7];
        playerDirection = messageList[8];

        init();

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
                    if (playerType == "PLAYER1") {   
                        draw(state[1], state[2], SNAKE_COLOR);
                    }   
                    else {
                        draw(state[1], state[2], OPPONENT_SNAKE_COLOR);
                    }  
                    
                    break;
                case("PLAYER2"):
                    if (playerType == "PLAYER2") {   
                        draw(state[1], state[2], SNAKE_COLOR);
                    }   
                    else {
                        draw(state[1], state[2], OPPONENT_SNAKE_COLOR);
                    }  
                    break;
                case("SCORE1"):
                    playerScore = state[1];
                    updateScoreBoard();
                    break;
                case("SCORE2"):
                    opponentScore = state[1];
                    updateScoreBoard();
                    break;
            }
        }

    } else if (messageList[0] === "NEWGAME") {
        comparePlayersScore();
        displayWinner();
        gameCtx.clearRect(0,0,gameCanvas.width,gameCanvas.height);
    }
}

function updateCanvas(col, row) {
    gameCanvas.height = col * CELL_PX;
    gameCanvas.width = row * CELL_PX;

    scoreCanvas.width = col / 4 * CELL_PX;
    scoreCanvas.width = row * CELL_PX;
}

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

function connect() {
    Server = new FancyWebSocket('ws://' + document.getElementById('ip').value + ':' + document.getElementById('port').value);
    
    Server.bind('open', function() {
        // document.getElementById('cnt-btn').disabled = true;

        gameCanvas = document.getElementById("canvas-game");
        scoreCanvas = document.getElementById("canvas-score");

        playerID = document.getElementById('player-id').value;
        
        Server.send("message", "INIT:" + playerID);

    });

    Server.bind('close', function(data) {
        document.getElementById('cnt-btn').disabled = false;
    });

    Server.bind('message', receive);

    Server.connect();
}

// Updates the player's new direction and also sends the server the direction as well
function sendDirection(direction) {
    player.direction = direction;
    Server.send("message", "MOVE:" + playerNumber + ":" + direction);
}

function draw(x, y, color) {
   gameCtx.fillStyle = color;
   gameCtx.fillRect(x * CELL_PX, y * CELL_PX, CELL_PX, CELL_PX); 
}


function erase(x, y) {
   draw(x,y, BG_COLOR);
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

function displayWinner(text) {
    gameCtx.clearRect(0,0,gameCanvas.width,gameCanvas.height);
    gameCtx.font = "40px Ariel";
    gameCtx.fillStyle = "red";
    gameCtx.textAlign = "center";
    var winner = comparePlayersScore();
    gameCtx.fillText("GAME OVER: " + winner, gameCanvas.width/2, gameCanvas.height/2);
}
