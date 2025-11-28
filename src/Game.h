#ifndef GAME_H
#define GAME_H

#include "ChessBoard.h"
#include <SFML/Graphics.hpp>
#include <memory>

class Game {
public:
    Game();
    ~Game();

    bool initialize();
    void run();

private:
    void handleEvents();
    void update();
    void render();
    void cleanup();  // This should remain private

    sf::RenderWindow* window_;
    ChessBoard* board_;
};

#endif