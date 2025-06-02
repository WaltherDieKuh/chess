#pragma once
#include "tile.h"
#include <string>
#include <map>
#include <vector>

class FENParser {
private:
    std::map<char, PieceType> charToPieceType = {
        {'p', PieceType::PAWN}, {'r', PieceType::ROOK}, {'n', PieceType::KNIGHT},
        {'b', PieceType::BISHOP}, {'q', PieceType::QUEEN}, {'k', PieceType::KING},
        {'P', PieceType::PAWN}, {'R', PieceType::ROOK}, {'N', PieceType::KNIGHT},
        {'B', PieceType::BISHOP}, {'Q', PieceType::QUEEN}, {'K', PieceType::KING}
    };

public:
    struct Position {
        PieceType piece;
        PieceColor color;
        bool isEmpty;
        
        Position() : piece(PieceType::NONE), color(PieceColor::WHITE), isEmpty(true) {}
        Position(PieceType p, PieceColor c) : piece(p), color(c), isEmpty(false) {}
    };
    
    std::vector<std::vector<Position>> parseFEN(const std::string& fen);
    std::string positionToFEN(const std::vector<std::vector<Position>>& board);
    
private:
    PieceColor getColorFromChar(char c);
    PieceType getPieceFromChar(char c);
};