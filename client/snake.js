var COL = 90;
var ROW = 60;
var CELL_PX = 10;
var PLAYER_ONE_COLOR = 'green';
var PLAYER_TWO_COLOR = 'red';
var BG_COLOR = 'white';

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

function getPlayer(id) {
    if (playerOne.id === id) return playerOne;
    else return playerTwo;
}

function receive(message) {
    var messageList = message.split(":");

    console.log(messageList[0])

    if (messageList[0] === "WELCOME") {
        init();
    } else if (messageList[0] === "INIT") {
        for (var i = 2; i < messageList.length; i+=2) {
            draw({x: messageList[i], y: messageList[i+1]}, getPlayer(messageList[1]).color);
        }
    } else if (messageList[0] === "ERROR") {
        /* DO NOTHING */
    } else if (messageList[0] === "READY") {
        /* DO NOTHING */
    } else if (messageList[0] === "DRAW") {
        draw({x: messageList[2], y: messageList[3]}, getPlayer(messageList[1]).color);
    } else if (messageList[0] === "ERASE") {
        console.log("ASDFASDF");
        draw({x: messageList[2], y: messageList[3]}, BG_COLOR);
    } else if (messageList[0] === "SCORE") {
        getPlayer(messageList[1]).score = parseInt(messageList[2]);
        updateScoreBoard();
    } else if (messageList[0] === "GAME_OVER") {
        /* GAME OVER */
    }
}

function sendDirection(player, direction) {
    Server.send("MOVE " + player.id + " " + direction);
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

        // Space
        if (event.keyCode == 32) Server.send("START");
    });

    gameCanvas = document.getElementById("canvas-game");
    gameCtx = gameCanvas.getContext("2d");

    scoreCanvas = document.getElementById("canvas-score");
    scoreCtx = scoreCanvas.getContext("2d");

    updateScoreBoard();

}

function connect() {
    Server = new FancyWebSocket('ws://' + document.getElementById('ip').value + ':' + document.getElementById('port').value);
    
    Server.bind('open', function() {
        // document.getElementById('cnt-btn').disabled = true;

        var playerOneId = document.getElementById('player-1-id').value;
        var playerTwoId = document.getElementById('player-2-id').value;
        playerOne = {id: playerOneId, color: PLAYER_ONE_COLOR, score: 0};
        playerTwo = {id: playerTwoId, color: PLAYER_TWO_COLOR, score: 0};

        Server.send("CONNECT " + playerOne.id + " " + playerTwo.id);
    });

    Server.bind('close', function(data) {
        document.getElementById('cnt-btn').disabled = false;
    });

    Server.bind('message', receive);

    Server.connect();
}

function draw(cell, color) {
   gameCtx.fillStyle = color;
   gameCtx.fillRect(cell.x * CELL_PX, cell.y * CELL_PX, CELL_PX, CELL_PX); 
}

function updateScoreBoard() {
    // If not a new game, then clear the canvas to update the score
    if(!newGame) {
        scoreCtx.clearRect(0,0,1100,100);
    }
    else {
        newGame = false;
    }
    
    scoreCtx.font = "30px Ariel";
    scoreCtx.fillText(playerOne.id + "'s Score: " + playerOne.score, 100,30);
    scoreCtx.fillText(playerTwo.id + "'s Score: " + playerTwo.score, 800,30);
    
}