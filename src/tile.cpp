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
    // Rufe die erweiterte Version mit Standardwerten für die neuen Parameter auf
    drawTile(shaderProgram, VAO, false, false, false);
}

void Tile::drawTile(unsigned int shaderProgram, unsigned int VAO, bool selected, bool hovered, bool isValidMoveTarget) const {
    // Setze Tile-Position als Uniform
    int posLocation = glGetUniformLocation(shaderProgram, "tilePosition");
    glUniform2f(posLocation, getWorldX(), getWorldY());
    
    // Bestimme Tile-Farbe (mit Highlights für Auswahl/Hover)
    float r, g, b;
    if (selected) { // Das Feld, auf dem die ausgewählte Figur steht
        r = 0.6f; g = 0.6f; b = 0.2f; // Ein etwas anderes Gelb für die Auswahl der Figur
    } else if (hovered) { // Das Feld, über dem die Maus schwebt
        r = 0.2f; g = 0.5f; b = 0.2f; // Ein leichtes Grün für Hover
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
    bool hasPieceOnTile = (piece != PieceType::NONE && pieceTexture != nullptr);
    glUniform1i(hasPieceLocation, hasPieceOnTile ? 1 : 0);
    
    if (hasPieceOnTile) {
        pieceTexture->bind(0);
        int textureLocation = glGetUniformLocation(shaderProgram, "pieceTexture");
        glUniform1i(textureLocation, 0);
    }

    // Setze Uniform für das Hervorheben möglicher Züge
    int isPossibleMoveLocation = glGetUniformLocation(shaderProgram, "isPossibleMoveHighlight");
    glUniform1i(isPossibleMoveLocation, isValidMoveTarget ? 1 : 0);
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

float Tile::getWorldX() const {
    return (boardX - 3.5f) * 0.25f;
}

float Tile::getWorldY() const {
    return (boardY - 3.5f) * 0.25f;
}