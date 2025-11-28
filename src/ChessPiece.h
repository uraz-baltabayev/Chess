#pragma once
#include <string>
#include <memory>

enum class PieceType {
    NONE = 0,
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING
};

enum class PieceColor {
    NONE = 0,
    WHITE,
    BLACK
};

class ChessPiece {
public:
    ChessPiece(PieceType type, PieceColor color, int row, int col);
    
    PieceType getType() const { return type_; }
    PieceColor getColor() const { return color_; }
    int getRow() const { return row_; }
    int getCol() const { return col_; }
    void setPosition(int row, int col) { row_ = row; col_ = col; }
    bool hasMoved() const { return hasMoved_; }
    void setMoved(bool moved) { hasMoved_ = moved; }
    
    char getSymbol() const;
    std::string getName() const;
    bool isValidMove(int toRow, int toCol, const std::shared_ptr<ChessPiece> board[8][8]) const;
    
private:
    bool isValidPawnMove(int toRow, int toCol, const std::shared_ptr<ChessPiece> board[8][8]) const;
    bool isValidRookMove(int toRow, int toCol, const std::shared_ptr<ChessPiece> board[8][8]) const;
    bool isValidKnightMove(int toRow, int toCol) const;
    bool isValidBishopMove(int toRow, int toCol, const std::shared_ptr<ChessPiece> board[8][8]) const;
    bool isValidQueenMove(int toRow, int toCol, const std::shared_ptr<ChessPiece> board[8][8]) const;
    bool isValidKingMove(int toRow, int toCol) const;
    
    bool isPathClear(int toRow, int toCol, const std::shared_ptr<ChessPiece> board[8][8]) const;
    
    PieceType type_;
    PieceColor color_;
    int row_;
    int col_;
    bool hasMoved_;
};