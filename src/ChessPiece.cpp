#include "ChessPiece.h"
#include <cmath>
#include <cctype>

ChessPiece::ChessPiece(PieceType type, PieceColor color, int row, int col)
    : type_(type), color_(color), row_(row), col_(col), hasMoved_(false) {}

char ChessPiece::getSymbol() const {
    if (type_ == PieceType::NONE) return '.';
    
    char symbol;
    switch(type_) {
        case PieceType::PAWN: symbol = 'P'; break;
        case PieceType::ROOK: symbol = 'R'; break;
        case PieceType::KNIGHT: symbol = 'N'; break;
        case PieceType::BISHOP: symbol = 'B'; break;
        case PieceType::QUEEN: symbol = 'Q'; break;
        case PieceType::KING: symbol = 'K'; break;
        default: symbol = '?';
    }
    
    return (color_ == PieceColor::WHITE) ? symbol : std::tolower(static_cast<unsigned char>(symbol));
}

std::string ChessPiece::getName() const {
    switch(type_) {
        case PieceType::PAWN: return "Pawn";
        case PieceType::ROOK: return "Rook";
        case PieceType::KNIGHT: return "Knight";
        case PieceType::BISHOP: return "Bishop";
        case PieceType::QUEEN: return "Queen";
        case PieceType::KING: return "King";
        default: return "Empty";
    }
}

bool ChessPiece::isValidMove(int toRow, int toCol, const std::shared_ptr<ChessPiece> board[8][8]) const {
    if (type_ == PieceType::NONE) return false;
    if (toRow == row_ && toCol == col_) return false;
    if (toRow < 0 || toRow >= 8 || toCol < 0 || toCol >= 8) return false;
    
    // Check if target square has our own piece
    auto targetPiece = board[toRow][toCol];
    if (targetPiece && targetPiece->getColor() == color_) {
        return false;
    }
    
    switch(type_) {
        case PieceType::PAWN: return isValidPawnMove(toRow, toCol, board);
        case PieceType::ROOK: return isValidRookMove(toRow, toCol, board);
        case PieceType::KNIGHT: return isValidKnightMove(toRow, toCol);
        case PieceType::BISHOP: return isValidBishopMove(toRow, toCol, board);
        case PieceType::QUEEN: return isValidQueenMove(toRow, toCol, board);
        case PieceType::KING: return isValidKingMove(toRow, toCol);
        default: return false;
    }
}

bool ChessPiece::isValidPawnMove(int toRow, int toCol, const std::shared_ptr<ChessPiece> board[8][8]) const {
    int direction = (color_ == PieceColor::WHITE) ? -1 : 1;
    int rowDiff = toRow - row_;
    int colDiff = toCol - col_;
    
    auto targetPiece = board[toRow][toCol];
    
    // Forward move
    if (colDiff == 0) {
        // Single move forward
        if (rowDiff == direction && !targetPiece) {
            return true;
        }
        // Double move from starting position
        if (!hasMoved_ && rowDiff == 2 * direction && !targetPiece) {
            // Check if the square in between is empty
            int middleRow = row_ + direction;
            if (!board[middleRow][col_]) {
                return true;
            }
        }
    }
    // Capture move (diagonal)
    else if (std::abs(colDiff) == 1 && rowDiff == direction) {
        if (targetPiece && targetPiece->getColor() != color_) {
            return true;
        }
    }
    
    return false;
}

bool ChessPiece::isValidRookMove(int toRow, int toCol, const std::shared_ptr<ChessPiece> board[8][8]) const {
    int rowDiff = toRow - row_;
    int colDiff = toCol - col_;
    
    // Rook moves horizontally or vertically
    if (rowDiff != 0 && colDiff != 0) {
        return false;
    }
    
    return isPathClear(toRow, toCol, board);
}

bool ChessPiece::isValidKnightMove(int toRow, int toCol) const {
    int rowDiff = std::abs(toRow - row_);
    int colDiff = std::abs(toCol - col_);
    
    // Knight moves in L-shape: 2 in one direction, 1 in perpendicular
    return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
}

bool ChessPiece::isValidBishopMove(int toRow, int toCol, const std::shared_ptr<ChessPiece> board[8][8]) const {
    int rowDiff = toRow - row_;
    int colDiff = toCol - col_;
    
    // Bishop moves diagonally
    if (std::abs(rowDiff) != std::abs(colDiff)) {
        return false;
    }
    
    return isPathClear(toRow, toCol, board);
}

bool ChessPiece::isValidQueenMove(int toRow, int toCol, const std::shared_ptr<ChessPiece> board[8][8]) const {
    int rowDiff = toRow - row_;
    int colDiff = toCol - col_;
    
    // Queen moves like rook or bishop
    bool isRookMove = (rowDiff == 0 || colDiff == 0);
    bool isBishopMove = (std::abs(rowDiff) == std::abs(colDiff));
    
    if (!isRookMove && !isBishopMove) {
        return false;
    }
    
    return isPathClear(toRow, toCol, board);
}

bool ChessPiece::isValidKingMove(int toRow, int toCol) const {
    int rowDiff = std::abs(toRow - row_);
    int colDiff = std::abs(toCol - col_);
    
    // King moves one square in any direction
    return rowDiff <= 1 && colDiff <= 1;
}

bool ChessPiece::isPathClear(int toRow, int toCol, const std::shared_ptr<ChessPiece> board[8][8]) const {
    int rowStep = 0, colStep = 0;
    
    if (toRow > row_) rowStep = 1;
    else if (toRow < row_) rowStep = -1;
    
    if (toCol > col_) colStep = 1;
    else if (toCol < col_) colStep = -1;
    
    int currentRow = row_ + rowStep;
    int currentCol = col_ + colStep;
    
    // Check all squares between current position and target (excluding target)
    while (currentRow != toRow || currentCol != toCol) {
        if (board[currentRow][currentCol]) {
            return false; // Path is blocked
        }
        currentRow += rowStep;
        currentCol += colStep;
    }
    
    return true;
}