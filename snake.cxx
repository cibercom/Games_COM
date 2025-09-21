#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <time.h>

//use const int so that the value cannot be changed.
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const int GRID_SIZE = 20;
const int GAME_SPEED = 150;

//array set to 100, indicate the maximu length for the snake is 100 only.
int snakeX[100], snakeY[100];
// Assigning variables.
int snakeLength;
int fruitX, fruitY;
int score;
int direction;
int gameOver;

//game window create by SDL
SDL_Window* window = NULL;
//this pointer indicates the rendere associated with the game window.
SDL_Renderer* renderer = NULL;

//error checking
bool init() {
    //Obtaining the error by using the SDL_GetError()
    //Check if SDL initialization with video support was succesful. If there is error then print message. 
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    
    //This will create a window with the words Snake Game, but if failed will print the error message.
    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    //Will create renderer that associates with the game window, but if failed will print the error message.
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

//cleaning up and close the SDL resources when game finished. It destroys the renderer and windwow after that quit the program. 
void close() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


//capture the keyboard input
void handleInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        //check any quitting game event, for example closing window, if yes then game over.
        if (event.type == SDL_QUIT) {
            gameOver = true;
        }
        //check which key has been input.
        else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    if (direction != 2)
                        direction = 0;
                    break;
                case SDLK_DOWN:
                    if (direction != 0)
                        direction = 2;
                    break;
                case SDLK_LEFT:
                    if (direction != 1)
                        direction = 3;
                    break;
                case SDLK_RIGHT:
                    if (direction != 3)
                        direction = 1;
                    break;
            }
        }
    }
}

//generate the fruit position randomly
void generateFruit() {
    fruitX = rand() % (WINDOW_WIDTH / GRID_SIZE);
    fruitY = rand() % (WINDOW_HEIGHT / GRID_SIZE);
}


//move the snake
void moveSnake() {
    for (int i = snakeLength; i > 0; --i) {
        snakeX[i] = snakeX[i - 1];
        snakeY[i] = snakeY[i - 1];
    }

    switch (direction) {
        case 0: // Up
            snakeY[0] -= 1;
            break;
        case 1: // Right
            snakeX[0] += 1;
            break;
        case 2: // Down
            snakeY[0] += 1;
            break;
        case 3: // Left
            snakeX[0] -= 1;
            break;
    }

    //check if the snake hit the boudary of the window, if yes then game over.
    if (snakeX[0] < 0 || snakeX[0] >= WINDOW_WIDTH / GRID_SIZE || snakeY[0] < 0 || snakeY[0] >= WINDOW_HEIGHT / GRID_SIZE) {
        gameOver = true;
    }

    //check collion between head and body, if yes then game over.
    for (int i = 1; i < snakeLength; ++i) {
        if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
            gameOver = true;
        }
    }

    //check if the snake's head collide with the fruit, if yes the score will increase.
    if (snakeX[0] == fruitX && snakeY[0] == fruitY) {
        score += 1;
        snakeLength++;
        generateFruit();
    }
}



//render everything, the snake, the fruit 
void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (int i = 0; i < snakeLength; ++i) {
        SDL_Rect rect = { snakeX[i] * GRID_SIZE, snakeY[i] * GRID_SIZE, GRID_SIZE, GRID_SIZE };
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect fruitRect = { fruitX * GRID_SIZE, fruitY * GRID_SIZE, GRID_SIZE, GRID_SIZE };
    SDL_RenderFillRect(renderer, &fruitRect);

    SDL_RenderPresent(renderer);
}


//everything is here
int main(int argc, char* argv[]) {
    srand(time(NULL));

    //initialize the SDl library and create the game window, if failed,  will exit the program and return to 1 which indicating an error.
    if (!init()) {
        return 1;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    //generate the initial fruit position
    generateFruit();

    //generate the initial position of the snake
    snakeX[0] = WINDOW_WIDTH / (2 * GRID_SIZE);
    snakeY[0] = WINDOW_HEIGHT / (2 * GRID_SIZE);
    //initial length
    snakeLength = 1;

    direction = -1;
    gameOver = false;
    score = 0;

    //records the current time
    Uint32 lastTime = SDL_GetTicks();

    //loop the game 
    while (!gameOver) {
        Uint32 currentTime = SDL_GetTicks();
        Uint32 deltaTime = currentTime - lastTime;

        if (deltaTime >= GAME_SPEED) {
            handleInput();
            moveSnake();
            render();

            lastTime = currentTime;
        }
    }
    //close the SDL resources before exit the program
    close();

    return 0;
}
