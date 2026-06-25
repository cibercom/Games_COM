#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

const int SIZE = 4;
const int TILE_SIZE = 160;
const int PADDING = 15;
const int BOARD_SIZE = SIZE * TILE_SIZE + (SIZE + 1) * PADDING;

class Game2048 {
private:
    std::vector<std::vector<int>> board;
    int score;
    bool moved;

    void addRandomTile() {
        std::vector<std::pair<int, int>> emptyTiles;
        for (int r = 0; r < SIZE; ++r) {
            for (int c = 0; c < SIZE; ++c) {
                if (board[r][c] == 0) emptyTiles.push_back({r, c});
            }
        }
        if (!emptyTiles.empty()) {
            int idx = rand() % emptyTiles.size();
            board[emptyTiles[idx].first][emptyTiles[idx].second] = (rand() % 10 == 0) ? 4 : 2;
        }
    }

    void rotate() {
        std::vector<std::vector<int>> temp = board;
        for (int r = 0; r < SIZE; ++r) {
            for (int c = 0; c < SIZE; ++c) {
                board[c][SIZE - 1 - r] = temp[r][c];
            }
        }
    }

    void slideLeft() {
        for (int r = 0; r < SIZE; ++r) {
            std::vector<int> row;
            for (int c = 0; c < SIZE; ++c) {
                if (board[r][c] != 0) row.push_back(board[r][c]);
            }
            
            std::vector<int> newRow;
            for (size_t i = 0; i < row.size(); ++i) {
                if (i + 1 < row.size() && row[i] == row[i + 1]) {
                    newRow.push_back(row[i] * 2);
                    score += row[i] * 2;
                    i++;
                    moved = true;
                } else {
                    newRow.push_back(row[i]);
                }
            }
            while (newRow.size() < SIZE) {
                newRow.push_back(0);
            }
            if (newRow != board[r]) moved = true;
            board[r] = newRow;
        }
    }

public:
    Game2048() {
        srand(time(0));
        restart();
    }

    void restart() {
        board = std::vector<std::vector<int>>(SIZE, std::vector<int>(SIZE, 0));
        score = 0;
        moved = false;
        addRandomTile();
        addRandomTile();
    }

    void move(int direction) {
        moved = false;
        // 0: Left, 1: Down, 2: Right, 3: Up
        for (int i = 0; i < direction; ++i) rotate();
        slideLeft();
        for (int i = 0; i < (4 - direction) % 4; ++i) rotate();
        
        if (moved) addRandomTile();
    }

    int getTile(int r, int c) const { return board[r][c]; }
    int getScore() const { return score; }

    sf::Color getTileColor(int value) {
        switch (value) {
            case 2:    return sf::Color(238, 228, 218);
            case 4:    return sf::Color(237, 224, 200);
            case 8:    return sf::Color(242, 177, 121);
            case 16:   return sf::Color(245, 149, 99);
            case 32:   return sf::Color(246, 124, 95);
            case 64:   return sf::Color(246, 94, 59);
            case 128:  return sf::Color(237, 207, 114);
            case 256:  return sf::Color(237, 204, 97);
            case 512:  return sf::Color(237, 200, 80);
            case 1024: return sf::Color(237, 197, 63);
            case 2048: return sf::Color(237, 194, 46);
            default:   return sf::Color(60, 58, 50);
        }
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(BOARD_SIZE, BOARD_SIZE + 100), "2048 - Cxxdroid SFML");
    window.setFramerateLimit(60);

    Game2048 game;
    sf::Font font;
    
    // Fallback path specifically mapped for Android systems within Cxxdroid
    if (!font.loadFromFile("/system/fonts/Roboto-Regular.ttf")) {
        if (!font.loadFromFile("sans-serif")) return -1; 
    }

    sf::Vector2i touchStart;
    bool isSwiping = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Keyboard controls (Desktop/Simulation mode)
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left)  game.move(0);
                if (event.key.code == sf::Keyboard::Down)  game.move(1);
                if (event.key.code == sf::Keyboard::Right) game.move(2);
                if (event.key.code == sf::Keyboard::Up)    game.move(3);
                if (event.key.code == sf::Keyboard::R)     game.restart();
            }

            // Mobile Touch Gestures (Swipe logic)
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                touchStart = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
                isSwiping = true;
            }

            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && isSwiping) {
                sf::Vector2i touchEnd(event.mouseButton.x, event.mouseButton.y);
                sf::Vector2i delta = touchEnd - touchStart;
                isSwiping = false;

                if (std::abs(delta.x) > 50 || std::abs(delta.y) > 50) {
                    if (std::abs(delta.x) > std::abs(delta.y)) {
                        if (delta.x > 0) game.move(2); // Swipe Right
                        else game.move(0);            // Swipe Left
                    } else {
                        if (delta.y > 0) game.move(1); // Swipe Down
                        else game.move(3);            // Swipe Up
                    }
                }
            }
        }

        window.clear(sf::Color(187, 173, 160));

        // Draw Board Background Grid
        for (int r = 0; r < SIZE; ++r) {
            for (int c = 0; c < SIZE; ++c) {
                sf::RectangleShape cell(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                cell.setPosition(PADDING + c * (TILE_SIZE + PADDING), 100 + PADDING + r * (TILE_SIZE + PADDING));
                cell.setFillColor(sf::Color(205, 193, 180));
                window.draw(cell);

                int val = game.getTile(r, c);
                if (val > 0) {
                    sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                    tile.setPosition(cell.getPosition());
                    tile.setFillColor(game.getTileColor(val));
                    window.draw(tile);

                    sf::Text text;
                    text.setFont(font);
                    text.setString(std::to_string(val));
                    text.setCharacterSize(val >= 1024 ? 40 : 50);
                    text.setFillColor(val <= 4 ? sf::Color(119, 110, 101) : sf::Color::White);
                    
                    // Centering calculation
                    sf::FloatRect textRect = text.getLocalBounds();
                    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                    text.setPosition(cell.getPosition().x + TILE_SIZE / 2.0f, cell.getPosition().y + TILE_SIZE / 2.0f);
                    
                    window.draw(text);
                }
            }
        }

        // Draw Score UI Header
        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setString("SCORE: " + std::to_string(game.getScore()));
        scoreText.setCharacterSize(40);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(PADDING, 25);
        window.draw(scoreText);

        window.display();
    }

    return 0;
}