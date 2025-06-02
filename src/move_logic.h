#pragma once

#include <vector>
#include <utility> // Für std::pair
#include "board.h" // Enthält ChessBoard, PieceType, PieceColor
#include "tile.h"  // Enthält Tile

namespace MoveLogic {

    /**
     * @brief Ermittelt alle gültigen Züge für eine Figur auf einer gegebenen Position.
     * 
     * @param startX Die X-Koordinate der Startposition der Figur.
     * @param startY Die Y-Koordinate der Startposition der Figur.
     * @param board Das aktuelle Schachbrett-Objekt.
     * @return Eine Liste von Paaren (x, y), die die gültigen Zielkoordinaten darstellen.
     */
    std::vector<std::pair<int, int>> getValidMoves(
        int startX,
        int startY,
        const ChessBoard& board
    );

} // namespace MoveLogic