var COL = 90;
var ROW = 60;
var INIT_SNAKE_LEN = 6;
var CELL_PX = 10;
var SNAKE_ONE_COLOR = 'green';
var SNAKE_TWO_COLOR = 'red';
var BG_COLOR = 'white';

var Server;
var canvas;
var ctx;
var d1 = 'right';
var d2 = 'left';
var food;

var snakeOne = [];
var snakeTwo = [];

function connect() {
    // Server = new FancyWebSocket('ws//' + document.getElementById('ip') + ':' + document.getElementById('port').value + ':' + document.getElementById('player-id').value);
    
    // Server.bind('open', function() {
    //     document.getElementById('cnt-btn').disabled = true;
    // });

    // Server.bind('close', function(data) {
    //     document.getElementById('cnt-btn').disabled = false;
    // });

    // Server.connect();
    init();

    var gameLoop = setInterval(moveSnakes, 60);
}

function init() {
    document.addEventListener('keydown', function(event) {
        // WASD
        if (event.keyCode == 87) d1 = 'up';
        else if (event.keyCode == 65) d1 = 'left'
        else if (event.keyCode == 83) d1 = 'down'
        else if (event.keyCode == 68) d1 = 'right';

        // Arrow keys
        if (event.keyCode == 38) d2 = 'up';
        else if (event.keyCode == 37) d2 = 'left'
        else if (event.keyCode == 40) d2 = 'down'
        else if (event.keyCode == 39) d2 = 'right';
    });

    createCanvas();
    generateSnakes();
    generateFood();
    drawSnakes();
}

function createCanvas() {
    canvas = document.createElement('canvas');
    canvas.id = 'snake-canvas-1';
    canvas.width = COL * CELL_PX;
    canvas.height = ROW * CELL_PX;
    canvas.style.border = '1px solid';
    ctx = canvas.getContext('2d');
    document.body.appendChild(canvas);
}

function generateSnakes() {
    for (var i = INIT_SNAKE_LEN-1; i >= 0; i--) {
        snakeOne.push({x: i, y: 0});
        snakeTwo.push({x: COL-1-i, y: ROW-1});
    }
}

function draw(cell, color) {
   ctx.fillStyle = color;
   ctx.fillRect(cell.x * CELL_PX, cell.y * CELL_PX, CELL_PX, CELL_PX); 
}

function drawSnakes() {
    for (var i = 0; i < snakeOne.length; i++) {
        var cell = snakeOne[i];
        draw(cell, SNAKE_ONE_COLOR);
        
    }

    for (var i = 0; i < snakeTwo.length; i++) {
        var cell = snakeTwo[i];
        draw(cell, SNAKE_TWO_COLOR);
    }
}

function moveSnakes() {
    var x1 = snakeOne[0].x;
    var y1 = snakeOne[0].y;

    var x2 = snakeTwo[0].x;
    var y2 = snakeTwo[0].y;

    if (d1 == 'right') x1++;
    else if (d1 == 'left') x1--;
    else if (d1 == 'up') y1--;
    else if (d1 == 'down') y1++;

    if (d2 == 'right') x2++;
    else if (d2 == 'left') x2--;
    else if (d2 == 'up') y2--;
    else if (d2 == 'down') y2++;

    var head1;
    if (checkCollision({x: x1, y: y1}, food)) {
        head1 = {x: x1, y: y1};
        generateFood();
    } else {
        head1 = snakeOne.pop();
        draw(head1, BG_COLOR);
        head1.x = x1;
        head1.y = y1;
    }
    snakeOne.unshift(head1);

    var head2;
    if (checkCollision({x: x2, y: y2}, food)) {
        head2 = {x: x2, y: y2};
        generateFood();
    } else {
        head2 = snakeTwo.pop();
        draw(head2, BG_COLOR);
        console.log(head2);
        head2.x = x2;
        head2.y = y2;
    }
    snakeTwo.unshift(head2);

    draw(head1, SNAKE_ONE_COLOR);
    draw(head2, SNAKE_TWO_COLOR);
}

function checkCollision(cell1, cell2) {
    return cell1.x == cell2.x && cell1.y == cell2.y;
}

function includesCell(cellArray, cell) {
    for (var i = 0; i < cellArray.length-1; i++) {
        if (checkCollision(cellArray[i], cell)) {
            return true;
        }
    }
    return false;
}

function generateFood() {
    food = null;
    var x, y, cell;

    while (food == null) {
        x = Math.floor((Math.random() * COL));
        y = Math.floor((Math.random() * ROW));

        cell = {x: x, y: y};
        if (!includesCell(snakeOne, cell) && !includesCell(snakeTwo, cell)) {
            food = cell;
        }
    }

    draw(food, 'blue');
}