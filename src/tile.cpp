#include "tile.h"
#include <iostream>

Tile::Tile(int x, int y, TileColor color) 
    : boardX(x), boardY(y), color(color), piece(PieceType::NONE), isSelected(false) {}

void Tile::drawTile(unsigned int shaderProgram, unsigned int VAO) const {
    // Setze Tile-Position als Uniform
    int posLocation = glGetUniformLocation(shaderProgram, "tilePosition");
    glUniform2f(posLocation, getWorldX(), getWorldY());
    
    // Setze Tile-Farbe als Uniform
    int colorLocation = glGetUniformLocation(shaderProgram, "tileColor");
    if (color == TileColor::WHITE) {
        glUniform3f(colorLocation, 0.8f, 0.7f, 0.6f); // Weiß
    } else {
        glUniform3f(colorLocation, 0.4f, 0.2f, 0.2f); // Schwarz
    }
    
    // Zeichne das Tile
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

float Tile::getWorldX() const {
    // Konvertiere Schachbrett-Koordinaten zu OpenGL-Koordinaten
    // -0.875 bis +0.875 für 8 Tiles mit Größe 0.25
    return (boardX - 3.5f) * 0.25f;
}

float Tile::getWorldY() const {
    return (boardY - 3.5f) * 0.25f;
}