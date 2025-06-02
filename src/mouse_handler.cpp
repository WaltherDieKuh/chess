#include "mouse_handler.h"
#include <iostream>
#include <cmath>

MouseHandler::MouseHandler(ChessBoard* board, PieceManager* manager) 
    : chessBoard(board), pieceManager(manager), isDragging(false),
      dragStartX(-1), dragStartY(-1), dragEndX(-1), dragEndY(-1),
      mouseX(0), mouseY(0), hoveredX(-1), hoveredY(-1),
      selectedX(-1), selectedY(-1) {}

void MouseHandler::handleMouseButton(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        
        auto [boardX, boardY] = screenToBoard(mouseX, mouseY, width, height);
        
        if (action == GLFW_PRESS) {
            if (isValidPosition(boardX, boardY)) {
                Tile* tile = chessBoard->getTile(boardX, boardY);
                if (tile && tile->getPiece() != PieceType::NONE) {
                    startDrag(boardX, boardY);
                    selectedX = boardX;
                    selectedY = boardY;
                }
            }
        } else if (action == GLFW_RELEASE) {
            if (isDragging && isValidPosition(boardX, boardY)) {
                endDrag(boardX, boardY);
            } else {
                cancelDrag();
            }
        }
    }
}

void MouseHandler::handleMouseMove(GLFWwindow* window, double xpos, double ypos) {
    mouseX = xpos;
    mouseY = ypos;
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    
    auto [boardX, boardY] = screenToBoard(xpos, ypos, width, height);
    
    if (isValidPosition(boardX, boardY)) {
        hoveredX = boardX;
        hoveredY = boardY;
    } else {
        hoveredX = -1;
        hoveredY = -1;
    }
}

std::pair<int, int> MouseHandler::screenToBoard(double mouseX, double mouseY, int windowWidth, int windowHeight) {
    // Konvertiere Screen-Koordinaten zu OpenGL-Koordinaten
    double glX = (2.0 * mouseX) / windowWidth - 1.0;
    double glY = 1.0 - (2.0 * mouseY) / windowHeight;
    
    // Berücksichtige Aspect Ratio
    float aspectRatio = (float)windowWidth / windowHeight;
    if (aspectRatio > 1.0f) {
        glX /= aspectRatio;
    } else {
        glY *= aspectRatio;
    }
    
    // Konvertiere zu Board-Koordinaten
    // Jedes Tile ist 0.25 breit/hoch, Board ist von -0.875 bis +0.875
    int boardX = (int)std::round((glX + 0.875) / 0.25);
    int boardY = (int)std::round((glY + 0.875) / 0.25);
    
    return {boardX, boardY};
}

bool MouseHandler::isValidPosition(int x, int y) {
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

void MouseHandler::startDrag(int x, int y) {
    isDragging = true;
    dragStartX = x;
    dragStartY = y;
    std::cout << "Started dragging piece from (" << x << ", " << y << ")" << std::endl;
}

void MouseHandler::endDrag(int x, int y) {
    if (!isDragging) return;
    
    dragEndX = x;
    dragEndY = y;
    
    // Führe den Zug aus
    if (dragStartX != dragEndX || dragStartY != dragEndY) {
        Tile* fromTile = chessBoard->getTile(dragStartX, dragStartY);
        Tile* toTile = chessBoard->getTile(dragEndX, dragEndY);
        
        if (fromTile && toTile) {
            // Figur bewegen
            PieceType piece = fromTile->getPiece();
            PieceColor color = fromTile->getPieceColor();
            auto texture = fromTile->getPieceTexture();
            
            // Setze Figur auf Zielfeld
            toTile->setPiece(piece, color, texture);
            
            // Entferne Figur vom Startfeld
            fromTile->removePiece();
            
            std::cout << "Moved piece from (" << dragStartX << ", " << dragStartY 
                      << ") to (" << dragEndX << ", " << dragEndY << ")" << std::endl;
        }
    }
    
    // Reset drag state
    isDragging = false;
    selectedX = -1;
    selectedY = -1;
    dragStartX = -1;
    dragStartY = -1;
}

void MouseHandler::cancelDrag() {
    isDragging = false;
    selectedX = -1;
    selectedY = -1;
    dragStartX = -1;
    dragStartY = -1;
}