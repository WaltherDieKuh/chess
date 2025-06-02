#include "tile.h"
#include <iostream>

Tile::Tile(int x, int y, TileColor color) 
    : boardX(x), boardY(y), color(color), piece(PieceType::NONE), 
      pieceColor(PieceColor::WHITE), isSelected(false) {}

void Tile::setPiece(PieceType pieceType, PieceColor color, std::shared_ptr<Texture> texture) {
    this->piece = pieceType;
    this->pieceColor = color;
    this->pieceTexture = texture;
}

void Tile::removePiece() {
    this->piece = PieceType::NONE;
    this->pieceTexture = nullptr;
}

void Tile::drawTile(unsigned int shaderProgram, unsigned int VAO) const {
    // Setze Tile-Position als Uniform
    int posLocation = glGetUniformLocation(shaderProgram, "tilePosition");
    glUniform2f(posLocation, getWorldX(), getWorldY());
    
    // Setze Tile-Farbe (wird immer als Hintergrund verwendet)
    int colorLocation = glGetUniformLocation(shaderProgram, "tileColor");
    if (color == TileColor::WHITE) {
        glUniform3f(colorLocation, 0.9f, 0.9f, 0.8f); // Helles beige
    } else {
        glUniform3f(colorLocation, 0.4f, 0.2f, 0.1f); // Dunkles braun
    }
    
    // Setze hasPiece Uniform
    int hasPieceLocation = glGetUniformLocation(shaderProgram, "hasPiece");
    bool hasPiece = (piece != PieceType::NONE && pieceTexture != nullptr);
    glUniform1i(hasPieceLocation, hasPiece ? 1 : 0);
    
    if (hasPiece) {
        // Binde Figuren-Textur
        pieceTexture->bind(0);
        int textureLocation = glGetUniformLocation(shaderProgram, "pieceTexture");
        glUniform1i(textureLocation, 0);
    }
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

float Tile::getWorldX() const {
    return (boardX - 3.5f) * 0.25f;
}

float Tile::getWorldY() const {
    return (boardY - 3.5f) * 0.25f;
}