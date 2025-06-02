#pragma once
#include <glad/glad.h>
#include "texture.h"
#include <memory>

enum class TileColor { WHITE, BLACK };
enum class PieceType { NONE, PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING };
enum class PieceColor { WHITE, BLACK };

class Tile {
private:
    int boardX, boardY;
    TileColor color;
    PieceType piece;
    PieceColor pieceColor;
    bool isSelected;
    std::shared_ptr<Texture> pieceTexture;

public:
    Tile(int x, int y, TileColor color);
    
    // Figuren-Verwaltung
    void setPiece(PieceType piece, PieceColor color, std::shared_ptr<Texture> texture);
    void removePiece();
    
    // Rendering
    void drawTile(unsigned int shaderProgram, unsigned int VAO) const;
    
    float getWorldX() const;
    float getWorldY() const;
};