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
    drawTile(shaderProgram, VAO, false, false);  // Call overloaded version
}

void Tile::drawTile(unsigned int shaderProgram, unsigned int VAO, bool selected, bool highlighted) const {
    // Setze Tile-Position als Uniform
    int posLocation = glGetUniformLocation(shaderProgram, "tilePosition");
    glUniform2f(posLocation, getWorldX(), getWorldY());
    
    // Bestimme Tile-Farbe (mit Highlights)
    float r, g, b;
    if (selected) {
        r = 1.0f; g = 1.0f; b = 0.0f; // Gelb für ausgewähltes Tile
    } else if (highlighted) {
        r = 0.0f; g = 1.0f; b = 0.0f; // Grün für gehovertes Tile
    } else if (color == TileColor::WHITE) {
        r = 0.9f; g = 0.9f; b = 0.8f; // Helles beige
    } else {
        r = 0.4f; g = 0.2f; b = 0.1f; // Dunkles braun
    }
    
    // Setze Tile-Farbe
    int colorLocation = glGetUniformLocation(shaderProgram, "tileColor");
    glUniform3f(colorLocation, r, g, b);
    
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
    
    // Zeichne das Tile
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

float Tile::getWorldX() const {
    return (boardX - 3.5f) * 0.25f;
}

float Tile::getWorldY() const {
    return (boardY - 3.5f) * 0.25f;
}