#include "ChessBoard.h"
#include <iostream>
#include <fstream>
#include <vector>

// ========== CONSTRUCTOR - UPDATED ==========
// CHANGES: Added initialization of enPassantTargetRow_ and enPassantTargetCol_ to -1
// WHY: Need to track which square is vulnerable to en passant capture
// ADDED: Castling tracking variables initialization
ChessBoard::ChessBoard() : currentPlayer_(PieceColor::WHITE), selectedRow_(-1), selectedCol_(-1), 
                          hasSelected_(false), enPassantTargetRow_(-1), enPassantTargetCol_(-1),
                          // ADDED: Castling initialization
                          whiteKingMoved_(false), blackKingMoved_(false),
                          whiteRookKingSideMoved_(false), whiteRookQueenSideMoved_(false),
                          blackRookKingSideMoved_(false), blackRookQueenSideMoved_(false) {
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

// ========== movePiece METHOD - CORRECTED ==========
// FIXED: Proper turn switching for both castling and regular moves
bool ChessBoard::movePiece(int fromRow, int fromCol, int toRow, int toCol) {
    auto piece = board_[fromRow][fromCol];
    if (!piece || piece->getColor() != currentPlayer_) return false;

    // Get valid moves first to check if castling is possible
    auto validMoves = getValidMoves(fromRow, fromCol);
    bool isValidMove = false;
    for (const auto& move : validMoves) {
        if (move.first == toRow && move.second == toCol) {
            isValidMove = true;
            break;
        }
    }
    if (!isValidMove) return false;

    bool isCastlingMove = false;

    // CHECK FOR CASTLING FIRST
    if (piece->getType() == PieceType::KING && std::abs(toCol - fromCol) == 2) {
        // This is a castling move
        isCastlingMove = true;
        if (toCol > fromCol) {
            // Kingside castling
            performCastleKingSide(currentPlayer_);
            std::cout << "Kingside castling!" << std::endl;
        } else {
            // Queenside castling
            performCastleQueenSide(currentPlayer_);
            std::cout << "Queenside castling!" << std::endl;
        }
        
        // Update king moved status
        if (currentPlayer_ == PieceColor::WHITE) {
            whiteKingMoved_ = true;
        } else {
            blackKingMoved_ = true;
        }
        
        clearEnPassantTarget();
    } else {
        // ========== REGULAR MOVE (NON-CASTLING) ==========
        
        // ========== EN PASSANT CAPTURE LOGIC ==========
        if (piece->getType() == PieceType::PAWN && 
            fromCol != toCol &&                    
            !board_[toRow][toCol]) {               
            if (toRow == enPassantTargetRow_ && toCol == enPassantTargetCol_) {
                int capturedPawnRow = fromRow;
                int capturedPawnCol = toCol;
                if (capturedPawnRow >= 0 && capturedPawnRow < 8 && capturedPawnCol >= 0 && capturedPawnCol < 8) {
                    board_[capturedPawnRow][capturedPawnCol] = nullptr;
                    std::cout << "En passant capture! Removed pawn at " << capturedPawnRow << "," << capturedPawnCol << std::endl;
                }
            }
        }

        // Track rook and king movement for castling
        if (piece->getType() == PieceType::KING) {
            if (currentPlayer_ == PieceColor::WHITE) {
                whiteKingMoved_ = true;
            } else {
                blackKingMoved_ = true;
            }
        }
        
        // Track rook movement
        if (piece->getType() == PieceType::ROOK) {
            if (fromRow == 0 && fromCol == 0) blackRookQueenSideMoved_ = true;
            if (fromRow == 0 && fromCol == 7) blackRookKingSideMoved_ = true;
            if (fromRow == 7 && fromCol == 0) whiteRookQueenSideMoved_ = true;
            if (fromRow == 7 && fromCol == 7) whiteRookKingSideMoved_ = true;
        }
        
        // Make the move for non-castling moves
        board_[toRow][toCol] = piece;
        board_[fromRow][fromCol] = nullptr;
        piece->setPosition(toRow, toCol);
        piece->setMoved(true);
        
        // ========== EN PASSANT TARGET SETTING ==========
        if (piece->getType() == PieceType::PAWN && std::abs(toRow - fromRow) == 2) {
            int enPassantRow = (currentPlayer_ == PieceColor::WHITE) ? toRow + 1 : toRow - 1;
            setEnPassantTarget(enPassantRow, toCol);
            std::cout << "En passant target set at: " << enPassantRow << ", " << toCol << std::endl;
        } else {
            clearEnPassantTarget();
        }
    }
    
    // FIXED: Switch player for ALL successful moves (both castling and regular)
    switchPlayer();
    
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

// ========== handleClick METHOD - CORRECTED ==========
// FIXED: Removed switchPlayer() call to prevent double switching
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
            // REMOVED: switchPlayer(); - Now handled in movePiece() for all moves
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

// ========== getValidMoves METHOD - UPDATED ==========
// CHANGES: Added en passant validation for pawn moves
// WHY: Need to filter which diagonal pawn moves are actually valid en passant captures
// ADDED: Castling moves for kings
std::vector<std::pair<int, int>> ChessBoard::getValidMoves(int row, int col) const {
    std::vector<std::pair<int, int>> moves;
    auto piece = board_[row][col];
    if (!piece) return moves;

    // ADDED: For kings, add castling moves if available
    if (piece->getType() == PieceType::KING) {
        // Add normal king moves
        for (int toRow = 0; toRow < 8; ++toRow) {
            for (int toCol = 0; toCol < 8; ++toCol) {
                if (piece->isValidMove(toRow, toCol, board_)) {
                    moves.emplace_back(toRow, toCol);
                }
            }
        }
        
        // ADD CASTLING MOVES
        if (canCastleKingSide(piece->getColor())) {
            int kingRow = (piece->getColor() == PieceColor::WHITE) ? 7 : 0;
            moves.emplace_back(kingRow, 6); // Kingside castling target
        }
        if (canCastleQueenSide(piece->getColor())) {
            int kingRow = (piece->getColor() == PieceColor::WHITE) ? 7 : 0;
            moves.emplace_back(kingRow, 2); // Queenside castling target
        }
    }
    // For pawns, handle en passant (existing code)
    else if (piece->getType() == PieceType::PAWN) {
        for (int toRow = 0; toRow < 8; ++toRow) {
            for (int toCol = 0; toCol < 8; ++toCol) {
                if (piece->isValidMove(toRow, toCol, board_)) {
                    if (col != toCol && !board_[toRow][toCol]) {
                        if (toRow == enPassantTargetRow_ && toCol == enPassantTargetCol_) {
                            moves.emplace_back(toRow, toCol);
                        }
                    } else {
                        moves.emplace_back(toRow, toCol);
                    }
                }
            }
        }
    } else {
        // For other pieces, use normal validation
        for (int toRow = 0; toRow < 8; ++toRow) {
            for (int toCol = 0; toCol < 8; ++toCol) {
                if (piece->isValidMove(toRow, toCol, board_)) {
                    moves.emplace_back(toRow, toCol);
                }
            }
        }
    }
    
    return moves;
}

// ADDED: Castling validation and execution methods

bool ChessBoard::canCastleKingSide(PieceColor color) const {
    int kingRow = (color == PieceColor::WHITE) ? 7 : 0;
    
    // Check if king has moved
    if ((color == PieceColor::WHITE && whiteKingMoved_) ||
        (color == PieceColor::BLACK && blackKingMoved_)) {
        return false;
    }
    
    // Check if kingside rook has moved
    if ((color == PieceColor::WHITE && whiteRookKingSideMoved_) ||
        (color == PieceColor::BLACK && blackRookKingSideMoved_)) {
        return false;
    }
    
    // Check if squares between king and rook are empty
    if (board_[kingRow][5] || board_[kingRow][6]) {
        return false;
    }
    
    // Check if king is in check
    if (isCheck(color)) {
        return false;
    }
    
    // Check if king passes through or lands on attacked squares
    if (isSquareUnderAttack(kingRow, 4, color) ||  // King's original position
        isSquareUnderAttack(kingRow, 5, color) ||  // Square king passes through
        isSquareUnderAttack(kingRow, 6, color)) {  // Square king lands on
        return false;
    }
    
    return true;
}

bool ChessBoard::canCastleQueenSide(PieceColor color) const {
    int kingRow = (color == PieceColor::WHITE) ? 7 : 0;
    
    // Check if king has moved
    if ((color == PieceColor::WHITE && whiteKingMoved_) ||
        (color == PieceColor::BLACK && blackKingMoved_)) {
        return false;
    }
    
    // Check if queenside rook has moved
    if ((color == PieceColor::WHITE && whiteRookQueenSideMoved_) ||
        (color == PieceColor::BLACK && blackRookQueenSideMoved_)) {
        return false;
    }
    
    // Check if squares between king and rook are empty
    if (board_[kingRow][1] || board_[kingRow][2] || board_[kingRow][3]) {
        return false;
    }
    
    // Check if king is in check
    if (isCheck(color)) {
        return false;
    }
    
    // Check if king passes through or lands on attacked squares
    if (isSquareUnderAttack(kingRow, 4, color) ||  // King's original position
        isSquareUnderAttack(kingRow, 3, color) ||  // Square king passes through
        isSquareUnderAttack(kingRow, 2, color)) {  // Square king lands on
        return false;
    }
    
    return true;
}

void ChessBoard::performCastleKingSide(PieceColor color) {
    int kingRow = (color == PieceColor::WHITE) ? 7 : 0;
    
    // Move king
    auto king = board_[kingRow][4];
    board_[kingRow][4] = nullptr;
    board_[kingRow][6] = king;
    king->setPosition(kingRow, 6);
    king->setMoved(true);
    
    // Move rook
    auto rook = board_[kingRow][7];
    board_[kingRow][7] = nullptr;
    board_[kingRow][5] = rook;
    rook->setPosition(kingRow, 5);
    rook->setMoved(true);
}

void ChessBoard::performCastleQueenSide(PieceColor color) {
    int kingRow = (color == PieceColor::WHITE) ? 7 : 0;
    
    // Move king
    auto king = board_[kingRow][4];
    board_[kingRow][4] = nullptr;
    board_[kingRow][2] = king;
    king->setPosition(kingRow, 2);
    king->setMoved(true);
    
    // Move rook
    auto rook = board_[kingRow][0];
    board_[kingRow][0] = nullptr;
    board_[kingRow][3] = rook;
    rook->setPosition(kingRow, 3);
    rook->setMoved(true);
}

bool ChessBoard::isSquareUnderAttack(int row, int col, PieceColor defenderColor) const {
    // Check if any opponent piece can attack this square
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            auto piece = board_[i][j];
            if (piece && piece->getColor() != defenderColor) {
                if (piece->isValidMove(row, col, board_)) {
                    return true;
                }
            }
        }
    }
    return false;
}

sf::Color ChessBoard::getSquareColor(int row, int col) const {
    return (row + col) % 2 == 0 ? sf::Color(240, 217, 181) : sf::Color(181, 136, 99);
}