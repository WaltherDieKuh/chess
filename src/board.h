#pragma once
#include "tile.h"
#include "piece_manager.h"
#include "fen_parser.h"
#include <vector>
#include <memory>

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
    void drawBoard(unsigned int shaderProgram, unsigned int VAO) const;
    Tile* getTile(int x, int y) const;
    
    // FEN-Funktionen
    std::string getCurrentFEN() const { return currentFEN; }
    void setPosition(const std::string& fen, PieceManager& pieceManager);
};