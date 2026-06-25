#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

// Window and board layout definitions
const int WINDOW_SIZE = 600;
const int CELL_SIZE = WINDOW_SIZE / 3;
const float LINE_THICKNESS = 10.f;

// Game state variables
enum CellState { EMPTY = 0, PLAYER_X, PLAYER_O };
std::vector<std::vector<CellState>> board(3, std::vector<CellState>(3, EMPTY));
CellState turn = PLAYER_X;
bool gameOver = false;
bool isDraw = false;

// Function to check for a winner or a draw
void checkGameState() {
    // Check rows and columns
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] != EMPTY && board[i][0] == board[i][1] && board[i][1] == board[i][2]) { gameOver = true; return; }
        if (board[0][i] != EMPTY && board[0][i] == board[1][i] && board[1][i] == board[2][i]) { gameOver = true; return; }
    }
    // Check diagonals
    if (board[0][0] != EMPTY && board[0][0] == board[1][1] && board[1][1] == board[2][2]) { gameOver = true; return; }
    if (board[0][2] != EMPTY && board[0][2] == board[1][1] && board[1][1] == board[2][0]) { gameOver = true; return; }

    // Check for draw
    bool emptyFound = false;
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            if (board[r][c] == EMPTY) emptyFound = true;
        }
    }
    if (!emptyFound) {
        gameOver = true;
        isDraw = true;
    }
}

// Reset the game board
void resetGame() {
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) board[r][c] = EMPTY;
    }
    turn = PLAYER_X;
    gameOver = false;
    isDraw = false;
}

int main() {
    // Creating the SFML Render Window
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Tic-Tac-Toe SFML", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    // Grid lines setup
    std::vector<sf::RectangleShape> lines;
    for (int i = 1; i < 3; ++i) {
        // Vertical lines
        sf::RectangleShape vLine(sf::Vector2f(LINE_THICKNESS, WINDOW_SIZE));
        vLine.setPosition(i * CELL_SIZE - LINE_THICKNESS / 2.f, 0);
        vLine.setFillColor(sf::Color(200, 200, 200));
        lines.push_back(vLine);

        // Horizontal lines
        sf::RectangleShape hLine(sf::Vector2f(WINDOW_SIZE, LINE_THICKNESS));
        hLine.setPosition(0, i * CELL_SIZE - LINE_THICKNESS / 2.f);
        hLine.setFillColor(sf::Color(200, 200, 200));
        lines.push_back(hLine);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Handle touch or mouse clicks
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (gameOver) {
                    resetGame();
                } else {
                    int col = event.mouseButton.x / CELL_SIZE;
                    int row = event.mouseButton.y / CELL_SIZE;

                    // Ensure click is within valid bounds and cell is empty
                    if (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == EMPTY) {
                        board[row][col] = turn;
                        checkGameState();
                        if (!gameOver) {
                            turn = (turn == PLAYER_X) ? PLAYER_O : PLAYER_X;
                        }
                    }
                }
            }
        }

        // Clear Screen
        // If game is over, tint background slightly to signal status
        if (gameOver && !isDraw) {
            window.clear(turn == PLAYER_X ? sf::Color(30, 50, 30) : sf::Color(50, 30, 30)); // Greenish for X win, Reddish for O win
        } else if (gameOver && isDraw) {
            window.clear(sf::Color(40, 40, 40)); // Dark grey for draw
        } else {
            window.clear(sf::Color(20, 20, 20)); // Main dark background
        }

        // Draw grid layout
        for (const auto& line : lines) {
            window.draw(line);
        }

        // Draw dynamic elements (X and O)
        for (int r = 0; r < 3; ++r) {
            for (int c = 0; c < 3; ++c) {
                float centerX = c * CELL_SIZE + CELL_SIZE / 2.f;
                float centerY = r * CELL_SIZE + CELL_SIZE / 2.f;

                if (board[r][c] == PLAYER_O) {
                    // Draw "O" shape
                    float radius = CELL_SIZE / 3.f;
                    sf::CircleShape circle(radius);
                    circle.setOrigin(radius, radius);
                    circle.setPosition(centerX, centerY);
                    circle.setFillColor(sf::Color::Transparent);
                    circle.setOutlineThickness(8.f);
                    circle.setOutlineColor(sf::Color::Red);
                    window.draw(circle);
                } 
                else if (board[r][c] == PLAYER_X) {
                    // Draw "X" shape using two intersecting thin rectangles
                    float size = CELL_SIZE * 0.6f;
                    sf::RectangleShape line1(sf::Vector2f(size, 8.f));
                    line1.setOrigin(size / 2.f, 4.f);
                    line1.setPosition(centerX, centerY);
                    line1.setRotation(45.f);
                    line1.setFillColor(sf::Color::Green);

                    sf::RectangleShape line2(sf::Vector2f(size, 8.f));
                    line2.setOrigin(size / 2.f, 4.f);
                    line2.setPosition(centerX, centerY);
                    line2.setRotation(-45.f);
                    line2.setFillColor(sf::Color::Green);

                    window.draw(line1);
                    window.draw(line2);
                }
            }
        }

        window.display();
    }

    return 0;
}