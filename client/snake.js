var CELL_PX = 10;
var PLAYER_ONE_COLOR = "green";
var PLAYER_TWO_COLOR = "red";
var FOOD_COLOR = "blue";
var BG_COLOR = "white";

var Server;
var gameCanvas;
var scoreCanvas;
var gameCtx;
var scoreCtx;
var connected;
var playerOne;
var playerTwo;
var d1 = 'R';
var d2 = 'L';

var newGame = true;

function receive(message) {
    console.log(message);
    var messageList = message.split(":");

    if (messageList[0] === "SETUP") {

        updateCanvas(Number(messageList[1]), Number(messageList[2]));
        init();

    } else if (messageList[0] === "STATE") {

        for (var i = 1; i < messageList.length; i++) {
            var state = messageList[i].split(",");

            switch(state[0]) {
                case("FOOD"):
                    draw({x: state[1], y: state[2]}, FOOD_COLOR);
                    console.log(FOOD_COLOR);
                    break;
                case("EMPTY"):
                    erase({x: state[1], y: state[2]});
                    break;
                case("PLAYER1"):
                    drawPlayer({x: state[1], y: state[2]}, playerOne);
                    break;
                case("PLAYER2"):
                    drawPlayer({x: state[1], y: state[2]}, playerTwo);
                    break;
                case("SCORE1"):
                    playerOne.score = state[1];
                    updateScoreBoard();
                    break;
                case("SCORE2"):
                    playerTwo.score = state[1];
                    updateScoreBoard();
                    break;
            }
        }

    } else if (messageList[0] === "NEWGAME") {
        comparePlayersScore();
        displayWinner();
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
        // WASD
        if (event.keyCode == 87) sendDirection(playerOne, 'D' ? 'D' : 'U');
        else if (event.keyCode == 65) sendDirection(playerOne, 'R' ? 'R' : 'L');
        else if (event.keyCode == 83) sendDirection(playerOne, 'U' ? 'U' : 'D');
        else if (event.keyCode == 68) sendDirection(playerOne, 'L' ? 'L' : 'R');

        // Arrow keys
        if (event.keyCode == 38) sendDirection(playerTwo, 'D' ? 'D' : 'U');
        else if (event.keyCode == 37) sendDirection(playerTwo, 'R' ? 'R' : 'L');
        else if (event.keyCode == 40) sendDirection(playerTwo, 'U' ? 'U' : 'D');
        else if (event.keyCode == 39) sendDirection(playerTwo, 'L' ? 'L' : 'L');
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

        var playerOneId = document.getElementById('player-1-id').value;
        var playerTwoId = document.getElementById('player-2-id').value;
        playerOne = {id: playerOneId, color: PLAYER_ONE_COLOR, score: 0};
        playerTwo = {id: playerTwoId, color: PLAYER_TWO_COLOR, score: 0};

        Server.send('message',"ASDF");

        Server.send("message", "INIT:" + playerOne.id);
        Server.send("message", "INIT:" + playerTwo.id);
    });

    Server.bind('close', function(data) {
        document.getElementById('cnt-btn').disabled = false;
    });

    Server.bind('message', receive);

    Server.connect();
}

function getPlayer(id) {
    if (playerOne.id === id) return playerOne;
    else return playerTwo;
}

function sendDirection(player, direction) {
    Server.send("message", "MOVE " + player.id + " " + direction);
}

function draw(cell, color) {
   gameCtx.fillStyle = color;
   gameCtx.fillRect(cell.x * CELL_PX, cell.y * CELL_PX, CELL_PX, CELL_PX); 
}

function drawPlayer(cell, player) {
    draw(cell, player.color);
}

function erase(cell) {
   draw(cell, BG_COLOR);
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
    scoreCtx.fillText(playerOne.id + "'s Score: " + playerOne.score, scoreCanvas.width*2/8, scoreCanvas.height/2);
    scoreCtx.fillText(playerTwo.id + "'s Score: " + playerTwo.score, scoreCanvas.width*6/8, scoreCanvas.height/2);
    
}

function comparePlayersScore() {
    if(playerOne.score > playerTwo.score) {
        return playerOne.id.toUpperCase() + " WINS!";
    }
    else if (playerOne.score < playerTwo.score) {
        return playerTwo.id.toUpperCase() + " WINS!";
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
