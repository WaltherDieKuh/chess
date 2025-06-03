#pragma once
#include <GLFW/glfw3.h>
#include "board.h"
#include "piece_manager.h"
#include <vector> // Hinzufügen für std::vector
#include <utility> // Hinzufügen für std::pair

class MouseHandler {
private:
    ChessBoard* chessBoard;
    PieceManager* pieceManager;
    
    // Drag & Drop State
    bool isDragging;
    int dragStartX, dragStartY;
    int dragEndX, dragEndY;
    double mouseX, mouseY;
    
    // Für Highlight
    int hoveredX, hoveredY;
    int selectedX, selectedY;
    std::vector<std::pair<int, int>> currentValidMoves; // Hinzufügen

public:
    MouseHandler(ChessBoard* board, PieceManager* manager);
    
    void handleMouseButton(GLFWwindow* window, int button, int action, int mods);
    void handleMouseMove(GLFWwindow* window, double xpos, double ypos);
    
    // Hilfsfunktionen
    std::pair<int, int> screenToBoard(double mouseX, double mouseY, int windowWidth, int windowHeight);
    bool isValidPosition(int x, int y);
    void startDrag(int x, int y);
    void endDrag(int x, int y);
    void cancelDrag();
    
    // Getters für Rendering
    bool getIsDragging() const { return isDragging; }
    std::pair<int, int> getDragStart() const { return {dragStartX, dragStartY}; }
    std::pair<int, int> getSelected() const { return {selectedX, selectedY}; }
    std::pair<int, int> getHovered() const { return {hoveredX, hoveredY}; }
    const std::vector<std::pair<int, int>>& getValidMoveHighlights() const; // Hinzufügen
};