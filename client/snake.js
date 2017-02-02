var COL = 90;
var ROW = 60;
var CELL_PX = 10;
var PLAYER_ONE_COLOR = 'green';
var PLAYER_TWO_COLOR = 'red';
var BG_COLOR = 'white';

var Server;
var canvas;
var ctx;
var connected;
var playerOne;
var playerTwo;
var d1 = 'R';
var d2 = 'L';

function getPlayer(id) {
    if (playerOne.id === id) return playerOne;
    else return playerTwo;
}

function receive(message) {
    var messageList = message.split(" ");
    if (message[0] === "WELCOME") {
        init();
    } else if (message[0] === "INIT") {
        for (var i = 1; i < messageList.length; i+=2) {
            draw({x: message[i], y: message[i+1]}, getPlayer(message[1]).color);
        }
    } else if (message[0] == "ERROR") {
        /* DO NOTHING */
    } else if (message[0] == "READY") {
        /* DO NOTHING */
    } else if (message[0] === "DRAW") { 
        draw({x: message[2], y: message[3]}, getPlayer(message[1]).color);
    } else if (essage[0] === "ERASE") {
        draw({x: message[2], y: message[3]}, "BG_COLOR");
    } else if (message[0] === "SCORE") {
        getPlayer(message[1]).score = parseInt(message[2]);
    } else if (message[0] === "GAME_OVER") {
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

    createCanvas();
}

function connect() {
    Server = new FancyWebSocket('ws://' + document.getElementById('ip').value + ':' + document.getElementById('port').value);
    
    Server.bind('open', function() {
        document.getElementById('cnt-btn').disabled = true;
        connect = true;
    });

    Server.bind('close', function(data) {
        document.getElementById('cnt-btn').disabled = false;
    });

    Server.bind('message', receive);

    Server.connect();

    var playerOneId = document.getElementById('player-1-id').value;
    var playerTwoId = document.getElementById('player-2-id').value;
    playerOne = {id: playerOneId, color: PLAYER_ONE_COLOR, score: 0};
    playerTwo = {id: playerTwoId, color: PLAYER_TWO_COLOR, score: 0};

    if (connect == true) {
        Server.send("CONNECT " + playerOne.id + " " + playerTwo.id);
    }  
}

function draw(cell, color) {
   ctx.fillStyle = color;
   ctx.fillRect(cell.x * CELL_PX, cell.y * CELL_PX, CELL_PX, CELL_PX); 
}