#include "ChessBoard.h"
#include <iostream>
#include <fstream>
#include <vector>

ChessBoard::ChessBoard() : currentPlayer_(PieceColor::WHITE), selectedRow_(-1), selectedCol_(-1), hasSelected_(false) {
    // Initialize board with nullptrs
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            board_[i][j] = nullptr;
        }
    }
    initializeBoard();
}

ChessBoard::~ChessBoard() {
    // Smart pointers will handle cleanup
}

void ChessBoard::initializeBoard() {
    // Set up pawns
    for (int col = 0; col < 8; ++col) {
        board_[1][col] = std::make_shared<ChessPiece>(PieceType::PAWN, PieceColor::BLACK, 1, col);
        board_[6][col] = std::make_shared<ChessPiece>(PieceType::PAWN, PieceColor::WHITE, 6, col);
    }
    
    // Set up other pieces
    // Black pieces
    board_[0][0] = std::make_shared<ChessPiece>(PieceType::ROOK, PieceColor::BLACK, 0, 0);
    board_[0][1] = std::make_shared<ChessPiece>(PieceType::KNIGHT, PieceColor::BLACK, 0, 1);
    board_[0][2] = std::make_shared<ChessPiece>(PieceType::BISHOP, PieceColor::BLACK, 0, 2);
    board_[0][3] = std::make_shared<ChessPiece>(PieceType::QUEEN, PieceColor::BLACK, 0, 3);
    board_[0][4] = std::make_shared<ChessPiece>(PieceType::KING, PieceColor::BLACK, 0, 4);
    board_[0][5] = std::make_shared<ChessPiece>(PieceType::BISHOP, PieceColor::BLACK, 0, 5);
    board_[0][6] = std::make_shared<ChessPiece>(PieceType::KNIGHT, PieceColor::BLACK, 0, 6);
    board_[0][7] = std::make_shared<ChessPiece>(PieceType::ROOK, PieceColor::BLACK, 0, 7);
    
    // White pieces
    board_[7][0] = std::make_shared<ChessPiece>(PieceType::ROOK, PieceColor::WHITE, 7, 0);
    board_[7][1] = std::make_shared<ChessPiece>(PieceType::KNIGHT, PieceColor::WHITE, 7, 1);
    board_[7][2] = std::make_shared<ChessPiece>(PieceType::BISHOP, PieceColor::WHITE, 7, 2);
    board_[7][3] = std::make_shared<ChessPiece>(PieceType::QUEEN, PieceColor::WHITE, 7, 3);
    board_[7][4] = std::make_shared<ChessPiece>(PieceType::KING, PieceColor::WHITE, 7, 4);
    board_[7][5] = std::make_shared<ChessPiece>(PieceType::BISHOP, PieceColor::WHITE, 7, 5);
    board_[7][6] = std::make_shared<ChessPiece>(PieceType::KNIGHT, PieceColor::WHITE, 7, 6);
    board_[7][7] = std::make_shared<ChessPiece>(PieceType::ROOK, PieceColor::WHITE, 7, 7);
}

bool ChessBoard::loadTextures() {
    // Try different possible paths for resources
    std::vector<std::string> possiblePaths = {
        "resources/",               // Build directory (after CMake copy)
        "../resources/",            // Relative from build directory
        "../../resources/",         // Further up if needed
        "../src/resources/",        // Development path
        "../../src/resources/"      // Further up development path
    };
    
    std::vector<std::pair<std::string, std::string>> textureFiles = {
        {"white_pawn", "wp.png"},
        {"white_rook", "wr.png"},
        {"white_knight", "wn.png"},
        {"white_bishop", "wb.png"},
        {"white_queen", "wq.png"},
        {"white_king", "wk.png"},
        {"black_pawn", "bp.png"},
        {"black_rook", "br.png"},
        {"black_knight", "bn.png"},
        {"black_bishop", "bb.png"},
        {"black_queen", "bq.png"},
        {"black_king", "bk.png"}
    };
    
    bool allLoaded = true;
    for (const auto& [name, filename] : textureFiles) {
        bool loaded = false;
        
        // Try each possible path
        for (const auto& path : possiblePaths) {
            std::string fullPath = path + filename;
            std::cout << "Trying to load texture: " << fullPath << std::endl;
            
            if (loadTexture(name, fullPath)) {
                std::cout << "Successfully loaded: " << fullPath << std::endl;
                loaded = true;
                break;
            }
        }
        
        if (!loaded) {
            std::cerr << "Failed to load texture: " << filename << " from any path" << std::endl;
            allLoaded = false;
        }
    }
    
    if (allLoaded) {
        std::cout << "All textures loaded successfully!" << std::endl;
    } else {
        std::cerr << "Some textures failed to load!" << std::endl;
    }
    
    return allLoaded;
}

bool ChessBoard::loadTexture(const std::string& name, const std::string& filename) {
    // Check if file exists
    std::ifstream file(filename);
    if (!file.good()) {
        std::cerr << "File does not exist: " << filename << std::endl;
        return false;
    }
    file.close();
    
    sf::Texture texture;
    if (texture.loadFromFile(filename)) {
        textures_[name] = texture;
        std::cout << "Texture '" << name << "' loaded successfully, size: " 
                  << texture.getSize().x << "x" << texture.getSize().y << std::endl;
        return true;
    }
    
    std::cerr << "SFML failed to load texture: " << filename << std::endl;
    return false;
}

std::shared_ptr<ChessPiece> ChessBoard::getPiece(int row, int col) const {
    if (row < 0 || row >= 8 || col < 0 || col >= 8) return nullptr;
    return board_[row][col];
}

bool ChessBoard::movePiece(int fromRow, int fromCol, int toRow, int toCol) {
    auto piece = board_[fromRow][fromCol];
    if (!piece || piece->getColor() != currentPlayer_) return false;
    
    if (!piece->isValidMove(toRow, toCol, board_)) return false;
    
    // Make the move
    board_[toRow][toCol] = piece;
    board_[fromRow][fromCol] = nullptr;
    piece->setPosition(toRow, toCol);
    piece->setMoved(true);
    
    return true;
}

bool ChessBoard::isCheck(PieceColor color) const {
    // Find the king
    int kingRow = -1, kingCol = -1;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            auto piece = board_[i][j];
            if (piece && piece->getType() == PieceType::KING && piece->getColor() == color) {
                kingRow = i;
                kingCol = j;
                break;
            }
        }
        if (kingRow != -1) break;
    }
    
    if (kingRow == -1) return false;
    
    // Check if any opponent piece can capture the king
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            auto piece = board_[i][j];
            if (piece && piece->getColor() != color) {
                if (piece->isValidMove(kingRow, kingCol, board_)) {
                    return true;
                }
            }
        }
    }
    
    return false;
}

bool ChessBoard::isCheckmate(PieceColor color) {
    if (!isCheck(color)) return false;
    
    // Try all possible moves for all pieces of the current color
    for (int fromRow = 0; fromRow < 8; ++fromRow) {
        for (int fromCol = 0; fromCol < 8; ++fromCol) {
            auto piece = board_[fromRow][fromCol];
            if (piece && piece->getColor() == color) {
                for (int toRow = 0; toRow < 8; ++toRow) {
                    for (int toCol = 0; toCol < 8; ++toCol) {
                        if (piece->isValidMove(toRow, toCol, board_)) {
                            // Try the move
                            auto temp = board_[toRow][toCol];
                            board_[toRow][toCol] = piece;
                            board_[fromRow][fromCol] = nullptr;
                            
                            bool stillInCheck = isCheck(color);
                            
                            // Undo the move
                            board_[fromRow][fromCol] = piece;
                            board_[toRow][toCol] = temp;
                            
                            if (!stillInCheck) return false;
                        }
                    }
                }
            }
        }
    }
    
    return true;
}

void ChessBoard::switchPlayer() {
    currentPlayer_ = (currentPlayer_ == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
}

void ChessBoard::draw(sf::RenderWindow& window) const {
    drawBoard(window);
    drawPieces(window);  // Draw pieces BEFORE selection and valid moves
    drawSelection(window);
    drawValidMoves(window);
}

void ChessBoard::drawBoard(sf::RenderWindow& window) const {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            sf::RectangleShape square(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
            square.setPosition(sf::Vector2f(
                BOARD_OFFSET_X + col * SQUARE_SIZE, 
                BOARD_OFFSET_Y + row * SQUARE_SIZE
            ));
            square.setFillColor(getSquareColor(row, col));
            window.draw(square);
        }
    }
}

void ChessBoard::drawPieces(sf::RenderWindow& window) const {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            auto piece = board_[row][col];
            if (piece) {
                std::string textureName;
                switch (piece->getType()) {
                    case PieceType::PAWN:
                        textureName = (piece->getColor() == PieceColor::WHITE) ? "white_pawn" : "black_pawn";
                        break;
                    case PieceType::ROOK:
                        textureName = (piece->getColor() == PieceColor::WHITE) ? "white_rook" : "black_rook";
                        break;
                    case PieceType::KNIGHT:
                        textureName = (piece->getColor() == PieceColor::WHITE) ? "white_knight" : "black_knight";
                        break;
                    case PieceType::BISHOP:
                        textureName = (piece->getColor() == PieceColor::WHITE) ? "white_bishop" : "black_bishop";
                        break;
                    case PieceType::QUEEN:
                        textureName = (piece->getColor() == PieceColor::WHITE) ? "white_queen" : "black_queen";
                        break;
                    case PieceType::KING:
                        textureName = (piece->getColor() == PieceColor::WHITE) ? "white_king" : "black_king";
                        break;
                    default:
                        continue;
                }
                
                auto it = textures_.find(textureName);
                if (it != textures_.end()) {
                    sf::Sprite sprite(it->second);
                    
                    // Simple positioning - FIXED for SFML3
                    float posX = BOARD_OFFSET_X + col * SQUARE_SIZE;
                    float posY = BOARD_OFFSET_Y + row * SQUARE_SIZE;
                    
                    sprite.setPosition(sf::Vector2f(posX, posY));  // FIXED: Use Vector2f
                    window.draw(sprite);
                }
            }
        }
    }
}

void ChessBoard::drawSelection(sf::RenderWindow& window) const {
    if (hasSelected_) {
        sf::RectangleShape highlight(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
        highlight.setPosition(sf::Vector2f(
            BOARD_OFFSET_X + selectedCol_ * SQUARE_SIZE, 
            BOARD_OFFSET_Y + selectedRow_ * SQUARE_SIZE
        ));
        highlight.setFillColor(sf::Color(255, 255, 0, 100)); // Semi-transparent yellow
        window.draw(highlight);
    }
}

void ChessBoard::drawValidMoves(sf::RenderWindow& window) const {
    if (hasSelected_) {
        auto validMoves = getValidMoves(selectedRow_, selectedCol_);
        for (const auto& move : validMoves) {
            sf::CircleShape circle(8);
            circle.setPosition(sf::Vector2f(
                BOARD_OFFSET_X + move.second * SQUARE_SIZE + SQUARE_SIZE/2 - 8, 
                BOARD_OFFSET_Y + move.first * SQUARE_SIZE + SQUARE_SIZE/2 - 8
            ));
            circle.setFillColor(sf::Color(0, 255, 0, 100)); // Semi-transparent green
            window.draw(circle);
        }
    }
}

void ChessBoard::handleClick(int x, int y) {
    int col = (x - BOARD_OFFSET_X) / SQUARE_SIZE;
    int row = (y - BOARD_OFFSET_Y) / SQUARE_SIZE;
    
    if (row < 0 || row >= 8 || col < 0 || col >= 8) return;
    
    if (hasSelected_) {
        if (movePiece(selectedRow_, selectedCol_, row, col)) {
            if (isCheckmate(currentPlayer_)) {
                std::cout << "Checkmate! " << (currentPlayer_ == PieceColor::WHITE ? "Black" : "White") << " wins!" << std::endl;
            } else if (isCheck(currentPlayer_)) {
                std::cout << "Check!" << std::endl;
            }
            switchPlayer();
        }
        hasSelected_ = false;
    } else {
        auto piece = board_[row][col];
        if (piece && piece->getColor() == currentPlayer_) {
            selectedRow_ = row;
            selectedCol_ = col;
            hasSelected_ = true;
        }
    }
}

std::vector<std::pair<int, int>> ChessBoard::getValidMoves(int row, int col) const {
    std::vector<std::pair<int, int>> moves;
    auto piece = board_[row][col];
    if (!piece) return moves;
    
    for (int toRow = 0; toRow < 8; ++toRow) {
        for (int toCol = 0; toCol < 8; ++toCol) {
            if (piece->isValidMove(toRow, toCol, board_)) {
                moves.emplace_back(toRow, toCol);
            }
        }
    }
    
    return moves;
}

sf::Color ChessBoard::getSquareColor(int row, int col) const {
    return (row + col) % 2 == 0 ? sf::Color(240, 217, 181) : sf::Color(181, 136, 99);
}