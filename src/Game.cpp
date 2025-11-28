#include "Game.h"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

Game::Game() : window_(nullptr), board_(nullptr) {}

Game::~Game() {
    cleanup();  // Destructor calls cleanup
}

bool Game::initialize() {
    // SFML3: VideoMode now takes Vector2u
    window_ = new sf::RenderWindow(sf::VideoMode(sf::Vector2u(800, 700)), "Chess Game - SFML3");
    if (!window_) {
        std::cerr << "Failed to create window" << std::endl;
        return false;
    }
    
    board_ = new ChessBoard();
    
    // Try to load textures
    if (!board_->loadTextures()) {
        std::cout << "Some textures failed to load, using fallback rendering" << std::endl;
    } else {
        std::cout << "All chess piece textures loaded successfully!" << std::endl;
    }
    
    std::cout << "Chess Game Initialized Successfully!" << std::endl;
    std::cout << "Click on a piece to select it, then click on a destination square to move." << std::endl;
    
    return true;
}

void Game::run() {
    while (window_->isOpen()) {
        handleEvents();
        update();
        render();
    }
}

void Game::handleEvents() {
    // SFML3: Event handling with correct enum usage
    for (auto event = window_->pollEvent(); event.has_value(); event = window_->pollEvent()) {
        // Handle window closed
        if (event->is<sf::Event::Closed>()) {
            window_->close();
        }
        
        // Handle mouse click
        if (auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
            // Use the correct enum value
            if (mouseEvent->button == sf::Mouse::Button::Left) {
                board_->handleClick(mouseEvent->position.x, mouseEvent->position.y);
            }
        }
        
        // Handle keyboard
        if (auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
            // Use the correct enum value
            if (keyEvent->code == sf::Keyboard::Key::Escape) {
                window_->close();
            }
        }
    }
}

void Game::update() {
    // Game logic updates can go here
}

void Game::render() {
    window_->clear(sf::Color(50, 50, 50));
    
    // Draw board and pieces
    board_->draw(*window_);
    
    window_->display();
}

void Game::cleanup() {
    if (board_) {
        delete board_;
        board_ = nullptr;
    }
    
    if (window_) {
        delete window_;
        window_ = nullptr;
    }
}