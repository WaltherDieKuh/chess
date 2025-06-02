#pragma once
#include "tile.h"
#include "piece_manager.h"
#include "fen_parser.h"
#include <vector>
#include <memory>

class MouseHandler; // Forward declaration

class ChessBoard {
private:
    std::vector<std::vector<std::unique_ptr<Tile>>> board;
    FENParser fenParser;
    std::string currentFEN;

public:
    ChessBoard();
    void initializeBoard();
    void setupFromFEN(const std::string& fen, PieceManager& pieceManager);
    void setupInitialPieces(PieceManager& pieceManager);
    void setPosition(const std::string& fen, PieceManager& pieceManager);  // Remove duplicate
    void drawBoard(unsigned int shaderProgram, unsigned int VAO) const;  // Original method
    void drawBoard(unsigned int shaderProgram, unsigned int VAO, const MouseHandler& mouseHandler) const;  // New method
    Tile* getTile(int x, int y) const;
    
    // FEN-Funktionen
    std::string getCurrentFEN() const { return currentFEN; }
};