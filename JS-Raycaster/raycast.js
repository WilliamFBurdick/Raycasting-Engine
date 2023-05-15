const TILE_SIZE = 64;
const MAP_NUM_ROWS = 11;
const MAP_NUM_COLS = 15;

const WINDOW_WIDTH = MAP_NUM_COLS * TILE_SIZE;
const WINDOW_HEIGHT = MAP_NUM_ROWS * TILE_SIZE;

const FOV_ANGLE = 60 * (Math.PI / 180);

const WALL_STRIP_WIDTH = 1;
const NUM_RAYS = WINDOW_WIDTH / WALL_STRIP_WIDTH; 

const MINIMAP_SCALE_FACTOR = 0.25;

class Map {
    constructor() {
        this.grid = [
            [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1],
            [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1],
            [1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            [1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
        ];
    }
    render() {
        for (var i = 0; i < MAP_NUM_ROWS; i++){
            for (var j = 0; j < MAP_NUM_COLS; j++){
                var tileX = j * TILE_SIZE;
                var tileY = i * TILE_SIZE;
                var tileColor = this.grid[i][j] == 1 ? "#222" : "#FFF";
                stroke("#222");
                fill(tileColor);
                rect(MINIMAP_SCALE_FACTOR * tileX, MINIMAP_SCALE_FACTOR * tileY, MINIMAP_SCALE_FACTOR * TILE_SIZE, MINIMAP_SCALE_FACTOR * TILE_SIZE);
            }
        }
    }
    hasWallAt(x, y){
        if (x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_WIDTH){
            return true;
        }
        var mapGridIndexX = Math.floor(x / TILE_SIZE);
        var mapGridIndexY = Math.floor(y / TILE_SIZE);
        return this.grid[mapGridIndexY][mapGridIndexX] !== 0;
    }
}

class Player{
    constructor() {
        this.x = WINDOW_WIDTH / 2;
        this.y = WINDOW_HEIGHT / 2;
        this.radius = 10;
        this.turnDirection = 0; // -1 for left, +1 for right
        this.walkDirection = 0; // -1 for back, +1 for forward
        this.rotation = Math.PI / 2;
        this.moveSpeed = 3.0;
        this.turnSpeed = 3 * (Math.PI / 180);
    }
    render(){
        noStroke();
        fill("red");
        circle(MINIMAP_SCALE_FACTOR * this.x, MINIMAP_SCALE_FACTOR * this.y, MINIMAP_SCALE_FACTOR * this.radius);
        stroke("red");
        line(MINIMAP_SCALE_FACTOR * this.x, MINIMAP_SCALE_FACTOR * this.y,
            MINIMAP_SCALE_FACTOR * this.x + MINIMAP_SCALE_FACTOR * 20 * Math.cos(this.rotation),
            MINIMAP_SCALE_FACTOR * this.y + MINIMAP_SCALE_FACTOR * 20 * Math.sin(this.rotation));
    }
    update(){
        this.rotation += (this.turnDirection * this.turnSpeed);

        var moveStep = this.walkDirection * this.moveSpeed;
        var newX = this.x + Math.cos(this.rotation) * moveStep;
        var newY = this.y + Math.sin(this.rotation) * moveStep;
        // only update player position if it's not a wall
        if (!grid.hasWallAt(newX, newY)) {
            this.x = newX;
            this.y  = newY;
        }
    }
}

class Ray {
    constructor(rayAngle) {
        this.rayAngle = normalizeAngle(rayAngle);
        this.wallHitX = 0;
        this.wallHitY = 0;
        this.distance = 0;
        this.wasHitVertical = false;

        this.isRayFacingDown = this.rayAngle > 0 && this.rayAngle < Math.PI;
        this.isRayFacingUp = !this.isRayFacingDown;

        this.isRayFacingRight = this.rayAngle < 0.5 * Math.PI || this.rayAngle > 1.5 * Math.PI;
        this.isRayFacingLeft = !this.isRayFacingRight;
    }
    render() {
        stroke("rgba(255, 0, 0, 0.3) ");
        line(MINIMAP_SCALE_FACTOR * player.x, MINIMAP_SCALE_FACTOR * player.y, 
            MINIMAP_SCALE_FACTOR * this.wallHitX, MINIMAP_SCALE_FACTOR * this.wallHitY);
    }
    cast(){
        var xintercept, yintercept;
        var xstep, ystep;
        /////////////////////////////////////
        // HORIZONTAL RAY-GRID INTERSECTION//
        /////////////////////////////////////
        var foundHorizWallHit = false;
        var horizWallHitX = 0;
        var horizWallHitY = 0;

        // find y-coordinate of closest horizontal grid intersection
        yintercept = Math.floor(player.y / TILE_SIZE) * TILE_SIZE;
        yintercept += (this.isRayFacingDown) ? TILE_SIZE : 0;
        // find x-coordinate of closest horizontal grid intersection
        xintercept = player.x + (yintercept - player.y) / Math.tan(this.rayAngle);

        // Calculate increment for xstep and ystep
        ystep = TILE_SIZE;
        ystep *= (this.isRayFacingUp) ? -1 : 1;

        xstep = ystep / Math.tan(this.rayAngle);
        xstep *= (this.isRayFacingLeft && xstep > 0) ? -1 : 1;
        xstep *= (this.isRayFacingRight && xstep < 0) ? -1 : 1;

        var nextHorizTouchX = xintercept;
        var nextHorizTouchY = yintercept;

        
        // Increment xstep and ystep until we find a wall
        while(nextHorizTouchX >= 0 && nextHorizTouchX <= WINDOW_WIDTH && nextHorizTouchY >= 0 && nextHorizTouchY <= WINDOW_HEIGHT){
            if (grid.hasWallAt(nextHorizTouchX, nextHorizTouchY - (this.isRayFacingUp ? 1 : 0))){
                foundHorizWallHit = true;
                horizWallHitX = nextHorizTouchX;
                horizWallHitY = nextHorizTouchY;

                break;
            } else {
                nextHorizTouchX += xstep;
                nextHorizTouchY += ystep;
            }
        }

        /////////////////////////////////////
        // VERTICAL RAY-GRID INTERSECTION//
        /////////////////////////////////////
        var foundVertWallHit = false;
        var vertWallHitX = 0;
        var vertWallHitY = 0;

        // find x-coordinate of closest vertical grid intersection
        xintercept = Math.floor(player.x / TILE_SIZE) * TILE_SIZE;
        xintercept += (this.isRayFacingRight) ? TILE_SIZE : 0;
        // find y-coordinate of closest horizontal grid intersection
        yintercept = player.y + (xintercept - player.x) * Math.tan(this.rayAngle);

        // Calculate increment for xstep and ystep
        xstep = TILE_SIZE;
        xstep *= (this.isRayFacingLeft) ? -1 : 1;

        ystep = TILE_SIZE * Math.tan(this.rayAngle);
        ystep *= (this.isRayFacingUp && ystep > 0) ? -1 : 1;
        ystep *= (this.isRayFacingDown && ystep < 0) ? -1 : 1;

        var nextVertTouchX = xintercept;
        var nextVertTouchY = yintercept;

        // Increment xstep and ystep until we find a wall
        while(nextVertTouchX >= 0 && nextVertTouchX <= WINDOW_WIDTH && nextVertTouchY >= 0 && nextVertTouchY <= WINDOW_HEIGHT){
            if (grid.hasWallAt(nextVertTouchX - (this.isRayFacingLeft ? 1 : 0), nextVertTouchY)){
                foundVertWallHit = true;
                vertWallHitX = nextVertTouchX;
                vertWallHitY = nextVertTouchY;

                break;
            } else {
                nextVertTouchX += xstep;
                nextVertTouchY += ystep;
            }
        }

        // Calculate distances. Choose smallest.
        var horizHitDistance = (foundHorizWallHit) ? distanceBetweenPoints(player.x, player.y, horizWallHitX, horizWallHitY)
        : Number.MAX_VALUE;
        var vertHitDistance = (foundVertWallHit) ? distanceBetweenPoints(player.x, player.y, vertWallHitX, vertWallHitY)
        : Number.MAX_VALUE;

        // Only store the closest hit
        if (vertHitDistance < horizHitDistance){
            this.wallHitX = vertWallHitX;
            this.wallHitY = vertWallHitY;
            this.distance = vertHitDistance;
            this.wasHitVertical = true;
        } else{
            this.wallHitX = horizWallHitX;
            this.wallHitY = horizWallHitY;
            this.distance = horizHitDistance;
            this.wasHitVertical = false;
        }
    }
}

var grid = new Map();
var player = new Player();
var rays = [];

function keyPressed(){
    if (keyCode == UP_ARROW){
        player.walkDirection = 1;
    } else if (keyCode == DOWN_ARROW){
        player.walkDirection = -1;
    } else if (keyCode == RIGHT_ARROW){
        player.turnDirection = 1;
    } else if (keyCode == LEFT_ARROW){
        player.turnDirection = -1;
    }
}

function keyReleased() {
    if (keyCode == UP_ARROW){
        player.walkDirection = 0;
    } else if (keyCode == DOWN_ARROW){
        player.walkDirection = 0;
    } else if (keyCode == RIGHT_ARROW){
        player.turnDirection = 0;
    } else if (keyCode == LEFT_ARROW){
        player.turnDirection = 0;
    }
}

function castAllRays(){
    // start first ray
    var rayAngle = player.rotation - (FOV_ANGLE / 2);
    rays = [];
    // loop all columns and cast arrays
    for (var i = 0; i < NUM_RAYS; i++){
        var ray = new Ray(rayAngle);
        ray.cast();
        rays.push(ray);
        rayAngle += FOV_ANGLE / NUM_RAYS;
    }
}

function render3DProjectionWalls(){
    for(var i = 0; i < NUM_RAYS; i++){
        var ray = rays[i];
        
        var rayDistance = ray.distance;
        rayDistance = rayDistance * Math.cos(ray.rayAngle - player.rotation);

        // calculate distance to projection plane
        var distanceProjectionPlane = (WINDOW_WIDTH / 2) / Math.tan(FOV_ANGLE / 2);

        // projected wall height
        var wallStripHeight = (TILE_SIZE / rayDistance) * distanceProjectionPlane;

        // compute transparency based on difference
        var alpha = 1.0;

        var colorIntensity = ray.wasHitVertical ? 255 : 180;

        // render rectangle with corrected height
        fill("rgba(" + colorIntensity + "," + colorIntensity + "," + colorIntensity +  ", " + alpha + ")");
        noStroke();
        rect(
            i * WALL_STRIP_WIDTH,
            (WINDOW_HEIGHT / 2) - (wallStripHeight / 2),
            WALL_STRIP_WIDTH,
            wallStripHeight
        );
    }
}

function distanceBetweenPoints(x1, y1, x2, y2){
    return Math.sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

function normalizeAngle(angle){
    angle = angle % (2 * Math.PI);
    if (angle < 0){
        angle += (2 * Math.PI);
    }
    return angle;
}

function setup() {
    createCanvas(WINDOW_WIDTH, WINDOW_HEIGHT);
}

function update() {
    player.update();
    castAllRays();
}

function draw() {
    clear("#212121");
    update();

    render3DProjectionWalls();

    grid.render();
    for (ray of rays){
        ray.render();
    }
    player.render();
}