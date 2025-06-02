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
    bool isHighlighted;
    std::shared_ptr<Texture> pieceTexture;

public:
    Tile(int x, int y, TileColor color);
    
    // Figuren-Verwaltung
    void setPiece(PieceType piece, PieceColor color, std::shared_ptr<Texture> texture);
    void removePiece();
    
    // Rendering
    void drawTile(unsigned int shaderProgram, unsigned int VAO) const;
    void drawTile(unsigned int shaderProgram, unsigned int VAO, bool selected, bool highlighted) const;
    
    // Getters
    int getBoardX() const { return boardX; }
    int getBoardY() const { return boardY; }
    TileColor getColor() const { return color; }
    PieceType getPiece() const { return piece; }
    PieceColor getPieceColor() const { return pieceColor; }
    std::shared_ptr<Texture> getPieceTexture() const { return pieceTexture; }
    
    // Setters
    void setSelected(bool selected) { isSelected = selected; }
    void setHighlighted(bool highlighted) { isHighlighted = highlighted; }
    
    float getWorldX() const;
    float getWorldY() const;
};