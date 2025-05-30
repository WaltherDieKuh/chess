#pragma once
#include <glad/glad.h>

enum class TileColor { WHITE, BLACK };
enum class PieceType { NONE, PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING };

class Tile {
private:
    int boardX, boardY;           // Schachbrett-Koordinaten (0-7)
    TileColor color;
    PieceType piece;
    bool isSelected;

public:
    Tile(int x, int y, TileColor color);
    
    // Getters
    int getBoardX() const { return boardX; }
    int getBoardY() const { return boardY; }
    TileColor getColor() const { return color; }
    PieceType getPiece() const { return piece; }
    bool getSelected() const { return isSelected; }
    
    // Setters
    void setPiece(PieceType piece) { this->piece = piece; }
    void setSelected(bool selected) { isSelected = selected; }
    
    // Rendering
    void drawTile(unsigned int shaderProgram, unsigned int VAO) const;
    
    // Koordinaten-Konvertierung (Schachbrett -> OpenGL)
    float getWorldX() const;
    float getWorldY() const;
};