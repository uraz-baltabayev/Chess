#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "ChessPiece.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <unordered_map>

class ChessBoard {
public:
    ChessBoard();
    ~ChessBoard();

    void initializeBoard();
    bool loadTextures();
    bool loadFont();
    std::shared_ptr<ChessPiece> getPiece(int row, int col) const;
    bool movePiece(int fromRow, int fromCol, int toRow, int toCol);
    bool isCheck(PieceColor color) const;
    bool isCheckmate(PieceColor color);
    void switchPlayer();
    void draw(sf::RenderWindow& window) const;
    void handleClick(int x, int y);
    std::vector<std::pair<int, int>> getValidMoves(int row, int col) const;

private:
    static const int SQUARE_SIZE = 80;
    static const int BOARD_OFFSET_X = 50;
    static const int BOARD_OFFSET_Y = 50;

    std::shared_ptr<ChessPiece> board_[8][8];
    PieceColor currentPlayer_;
    int selectedRow_;
    int selectedCol_;
    bool hasSelected_;
    std::unordered_map<std::string, sf::Texture> textures_;
    sf::Font font_;  // Add this line

    bool loadTexture(const std::string& name, const std::string& filename);
    void drawBoard(sf::RenderWindow& window) const;
    void drawPieces(sf::RenderWindow& window) const;
    void drawSelection(sf::RenderWindow& window) const;
    void drawValidMoves(sf::RenderWindow& window) const;
    sf::Color getSquareColor(int row, int col) const;
};

#endif