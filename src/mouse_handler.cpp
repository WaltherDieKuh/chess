#include "mouse_handler.h"
#include <iostream>
#include <cmath>
#include "move_logic.h"

MouseHandler::MouseHandler(ChessBoard* board, PieceManager* manager)
    : chessBoard(board), pieceManager(manager), isDragging(false),
      dragStartX(-1), dragStartY(-1), dragEndX(-1), dragEndY(-1),
      mouseX(0), mouseY(0), hoveredX(-1), hoveredY(-1),
      selectedX(-1), selectedY(-1) {} // currentValidMoves wird standardmäßig leer initialisiert

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
    // Konvertiere Screen-Koordinaten zu OpenGL-Koordinaten (-1 bis +1)
    double ndcX = (2.0 * mouseX) / windowWidth - 1.0;
    double ndcY = 1.0 - (2.0 * mouseY) / windowHeight; // Y ist invertiert (oben ist +1)
    
    float aspectRatio = (float)windowWidth / windowHeight;
    
    // Definiere die Grenzen des Schachbretts im Model-Space (bevor die Shader Aspect-Ratio-Korrektur angewendet wird)
    // Annahme: Das Brett ist 2.0x2.0 Einheiten groß und zentriert um den Ursprung.
    const float modelBoardMin = -1.0f;
    const float modelBoardMax = 1.0f;
    // const float modelBoardSize = modelBoardMax - modelBoardMin; // 2.0f // Nicht direkt benötigt in dieser Logik

    // Berechne die tatsächlichen visuellen Grenzen des Schachbretts im NDC-Raum
    // (nachdem die Aspect-Ratio-Korrektur, wie im Shader, auf das Brett angewendet wurde)
    float visualBoardLeft, visualBoardRight, visualBoardBottom, visualBoardTop;

    if (aspectRatio > 1.0f) {
        // Fenster ist breiter als hoch: X-Achse des Bretts wird komprimiert
        visualBoardLeft = modelBoardMin / aspectRatio;
        visualBoardRight = modelBoardMax / aspectRatio;
        visualBoardBottom = modelBoardMin;
        visualBoardTop = modelBoardMax;
    } else {
        // Fenster ist höher als breit (oder quadratisch): Y-Achse des Bretts wird komprimiert
        visualBoardLeft = modelBoardMin;
        visualBoardRight = modelBoardMax;
        visualBoardBottom = modelBoardMin * aspectRatio;
        visualBoardTop = modelBoardMax * aspectRatio;
    }
    
    // Prüfe, ob die NDC-Mauskoordinaten innerhalb dieser visuellen Grenzen liegen
    if (ndcX < visualBoardLeft || ndcX > visualBoardRight ||
        ndcY < visualBoardBottom || ndcY > visualBoardTop) {
        return {-1, -1}; // Außerhalb des sichtbaren Schachbretts
    }
    
    // Berechne die Größe einer einzelnen Kachel im NDC-Raum
    float ndcTileSizeX = (visualBoardRight - visualBoardLeft) / 8.0f;
    float ndcTileSizeY = (visualBoardTop - visualBoardBottom) / 8.0f;
    
    // Berechne Board-Koordinaten (0-7) basierend auf NDC-Koordinaten
    // (0,0) soll unten links sein
    int boardX = static_cast<int>((ndcX - visualBoardLeft) / ndcTileSizeX);
    int boardY = static_cast<int>((ndcY - visualBoardBottom) / ndcTileSizeY);

    // Debug-Ausgabe
    static int lastX = -1, lastY = -1;
    if ((boardX != lastX || boardY != lastY) && boardX >= 0 && boardX < 8 && boardY >= 0 && boardY < 8) {
        std::cout << "NDC: (" << ndcX << ", " << ndcY 
                  << "), VisualBoard L:" << visualBoardLeft << " R:" << visualBoardRight << " B:" << visualBoardBottom << " T:" << visualBoardTop
                  << ", TileSizeNDC: (" << ndcTileSizeX << ", " << ndcTileSizeY 
                  << ") -> Board(" << boardX << ", " << boardY << ")" << std::endl;
        lastX = boardX; lastY = boardY;
    }
    
    // Sichere Begrenzung (sollte durch die obige Prüfung bereits abgedeckt sein, aber schadet nicht)
    boardX = std::max(0, std::min(7, boardX));
    boardY = std::max(0, std::min(7, boardY));
    
    return {boardX, boardY};
}

bool MouseHandler::isValidPosition(int x, int y) {
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

void MouseHandler::startDrag(int x, int y) {
    isDragging = true;
    dragStartX = x;
    dragStartY = y;
    selectedX = x; // selectedX/Y auch hier setzen, falls nicht schon in handleMouseButton
    selectedY = y;

    currentValidMoves = MoveLogic::getValidMoves(x, y, *chessBoard); // Speichere gültige Züge
    if (currentValidMoves.empty()) {
        std::cout << "No valid moves for piece at (" << x << ", " << y << ")" << std::endl;
        // cancelDrag(); // Nicht unbedingt canceln, Auswahl bleibt bestehen, aber keine Highlights
        // return; // Nicht return, damit selectedX/Y gesetzt bleiben
    }
    std::cout << "Started dragging piece from (" << x << ", " << y << "). Valid moves: " << currentValidMoves.size() << std::endl;
}

void MouseHandler::endDrag(int x, int y) {
    if (!isDragging) return;
    
    dragEndX = x;
    dragEndY = y;
    
    bool moveWasValid = false;
    for (const auto& move : currentValidMoves) {
        if (move.first == dragEndX && move.second == dragEndY) {
            moveWasValid = true;
            break;
        }
    }

    if (moveWasValid && (dragStartX != dragEndX || dragStartY != dragEndY)) {
        Tile* fromTile = chessBoard->getTile(dragStartX, dragStartY);
        Tile* toTile = chessBoard->getTile(dragEndX, dragEndY);
        
        if (fromTile && toTile) {
            PieceType piece = fromTile->getPiece();
            PieceColor color = fromTile->getPieceColor();
            auto texture = fromTile->getPieceTexture();
            
            toTile->setPiece(piece, color, texture);
            fromTile->removePiece();
            
            std::cout << "Moved piece from (" << dragStartX << ", " << dragStartY 
                      << ") to (" << dragEndX << ", " << dragEndY << ")" << std::endl;
        }
    } else if (dragStartX == dragEndX && dragStartY == dragEndY) {
        // Klick auf dasselbe Feld, Auswahl aufheben oder andere Logik
        std::cout << "Clicked same tile, drag cancelled essentially." << std::endl;
    } else {
        std::cout << "Invalid move to (" << dragEndX << ", " << dragEndY << ")" << std::endl;
    }
    
    isDragging = false;
    selectedX = -1;
    selectedY = -1;
    dragStartX = -1;
    dragStartY = -1;
    currentValidMoves.clear(); // Gültige Züge zurücksetzen
}

void MouseHandler::cancelDrag() {
    isDragging = false;
    selectedX = -1;
    selectedY = -1;
    dragStartX = -1;
    dragStartY = -1;
    currentValidMoves.clear(); // Gültige Züge zurücksetzen
}

const std::vector<std::pair<int, int>>& MouseHandler::getValidMoveHighlights() const {
    return currentValidMoves;
}