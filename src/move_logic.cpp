#include "move_logic.h"
#include <iostream> // Für optionale Debug-Ausgaben

namespace MoveLogic {

std::vector<std::pair<int, int>> getValidMoves(
    int startX,
    int startY,
    const ChessBoard& board
) {
    std::vector<std::pair<int, int>> validMoves;
    const Tile* startTile = board.getTile(startX, startY);

    if (!startTile || startTile->getPiece() == PieceType::NONE) {
        return validMoves; // Keine Figur auf dem Startfeld oder ungültiges Feld
    }

    PieceType pieceType = startTile->getPiece();
    PieceColor pieceColor = startTile->getPieceColor();

    switch (pieceType) {
        case PieceType::PAWN: {
            int direction = (pieceColor == PieceColor::WHITE) ? 1 : -1; // Weiß zieht Y hoch, Schwarz Y runter

            // 1. Einfacher Vorwärtsschritt
            int oneStepY = startY + direction;
            if (oneStepY >= 0 && oneStepY < 8) {
                const Tile* targetTileOneStep = board.getTile(startX, oneStepY);
                if (targetTileOneStep && targetTileOneStep->getPiece() == PieceType::NONE) {
                    validMoves.push_back({startX, oneStepY});

                    // 2. Doppelter Vorwärtsschritt (nur vom Startfeld aus und wenn der erste Schritt frei ist)
                    bool isFirstMove = (pieceColor == PieceColor::WHITE && startY == 1) ||
                                       (pieceColor == PieceColor::BLACK && startY == 6);
                    if (isFirstMove) {
                        int twoStepsY = startY + 2 * direction;
                        if (twoStepsY >= 0 && twoStepsY < 8) {
                            const Tile* targetTileTwoSteps = board.getTile(startX, twoStepsY);
                            if (targetTileTwoSteps && targetTileTwoSteps->getPiece() == PieceType::NONE) {
                                validMoves.push_back({startX, twoStepsY});
                            }
                        }
                    }
                }
            }

            // 3. Schlagen (diagonal)
            int captureY = startY + direction;
            if (captureY >= 0 && captureY < 8) {
                // Schlagen nach links (aus Sicht der Figur)
                if (startX - 1 >= 0) {
                    const Tile* captureLeftTile = board.getTile(startX - 1, captureY);
                    if (captureLeftTile && captureLeftTile->getPiece() != PieceType::NONE &&
                        captureLeftTile->getPieceColor() != pieceColor) {
                        validMoves.push_back({startX - 1, captureY});
                    }
                }
                // Schlagen nach rechts (aus Sicht der Figur)
                if (startX + 1 < 8) {
                    const Tile* captureRightTile = board.getTile(startX + 1, captureY);
                    if (captureRightTile && captureRightTile->getPiece() != PieceType::NONE &&
                        captureRightTile->getPieceColor() != pieceColor) {
                        validMoves.push_back({startX + 1, captureY});
                    }
                }
            }
            // Hier später Logik für En Passant und Bauernumwandlung hinzufügen
            break;
        }
        case PieceType::ROOK:
            // Implementieren Sie hier die Zuglogik für den Turm
            // (horizontal und vertikal, bis blockiert oder Brettende)
            break;
        case PieceType::KNIGHT:
            // Implementieren Sie hier die Zuglogik für den Springer
            // (L-Form, kann über Figuren springen)
            break;
        case PieceType::BISHOP:
            // Implementieren Sie hier die Zuglogik für den Läufer
            // (diagonal, bis blockiert oder Brettende)
            break;
        case PieceType::QUEEN:
            // Implementieren Sie hier die Zuglogik für die Dame
            // (Kombination aus Turm und Läufer)
            break;
        case PieceType::KING:
            // Implementieren Sie hier die Zuglogik für den König
            // (ein Feld in jede Richtung, später Rochade)
            break;
        default:
            // Unbekannter Figurentyp oder NONE
            break;
    }

    return validMoves;
}

} // namespace MoveLogic