var server;

var COLS=0, ROWS=0;
var grid=[];
var EMPTY=0, FOOD=1, SNAKE=2, SNOKE=3;
var name="", nome="";	//Names of snake and snoke respectively
var scare=0, score=0;	//Scores for snake and snoke respectively
var KEY_LEFT=37, KEY_UP=38, KEY_RIGHT=39, KEY_DOWN=40;
var keystate;

function send(text){
	var d = new Date();
    Server.send ('message', text + ":AT:" + d.getTime());//d.toISOString().slice(0,-1));
}

function connect(){
    Server = new FancyWebSocket('ws://' + document.getElementById('ip').value + ':' + document.getElementById('port').value);
    Server.connect();

    Server.bind('open', function(){
        send("SETUP:" + document.getElementById('playerid').value);
        document.getElementById("cntBtn").disabled = true;
    });

    Server.bind('close', function( data ) {
        document.getElementById("cntBtn").disabled = false;
    });

    Server.bind('message', function(message){
        var messageList = message.split(':');
        if (messageList[0] === "SETUP"){	//messageList will look like SETUP:COLS:ROWS:SNAKE1NAME:SNAKE2NAME
        	COLS = parseInt(messageList[1]);
        	ROWS = parseInt(messageList[2]);
        	name = messageList[3];
        	nome = messageList[4];
        	main();
        }
        else if (messageList[0] === "STATE"){	//messageList will look like STATE:CELL,X,Y:CELL,X,Y:CELL,X,Y:CELL,X,Y:CELL,X,Y:SCORE1,0:SCORE2,0
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
        	scare = messageList[6].split(',')[1];
        	score = messageList[7].split(',')[1];
        }
        else if (messageList[0] === "NEWGAME"){
        	for (var x = 0; x < COLS; x++){				
				for (var y = 0; y < ROWS; y++){
					grid[x][y] = EMPTY;
				}
			}
        }
    });
}

function update(){
	if (keystate[KEY_UP])
		send("MOVE:UP");
	if (keystate[KEY_DOWN])
		send("MOVE:DOWN");
	if (keystate[KEY_LEFT])
		send("MOVE:LEFT");
	if (keystate[KEY_RIGHT])
		send("MOVE:RIGHT");
}

function draw(){
	var tw = canvas.width/COLS;
	var th = canvas.height/ROWS;

	for (var x=0; x < COLS; x++){
		for(var y = 0; y < ROWS; y++){
			switch(grid[x][y]){
				case EMPTY:
					ctx.fillStyle = "#fff";
					break;				
				case FOOD:
					ctx.fillStyle = "#0f0";
					break;
				case SNAKE:
					ctx.fillStyle = "#0ff";
					break;
				case SNOKE:
					ctx.fillStyle = "#f00";
					break;
			}		
			ctx.fillRect(x*tw, y*th, tw, th);
		}
	}
	ctx.fillStyle = "#000";
	ctx.fillText(name + ": " + scare, 10, canvas.height-10);
	ctx.fillText(nome + ": " + score, canvas.width-100, canvas.height-10);
}

function loop(){
	update();
	draw();

	window.requestAnimationFrame(loop);
}

function main(){
	canvas = document.createElement("canvas");
	canvas.width = COLS*20;
	canvas.height = ROWS*20;
	ctx = canvas.getContext("2d");
	document.body.appendChild(canvas);
	ctx.font = "20px Helvetica";

	keystate = {};
	document.addEventListener("keydown", function(evt) {
		keystate[evt.keyCode] = true;
	});
	document.addEventListener("keyup", function(evt) {
	    delete keystate[evt.keyCode];
	}); 

	for (var x = 0; x < COLS; x++){
		grid.push([]);
		for (var y = 0; y < ROWS; y++){
			grid[x].push(EMPTY);
		}
	}

	loop();
}
