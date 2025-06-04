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
                            if (targetTileTwoSteps && targetTileTwoSteps->getPiece() == PieceType::NONE && targetTileOneStep->getPiece() == PieceType::NONE) {
                                validMoves.push_back({startX, twoStepsY});
                            }
                        }
                    }
                }
            }

            // 3. Schlagen (diagonal)
            int captureY = startY + direction;
            if (captureY >= 0 && captureY < 8) {
                // Schlagen nach links
                if (startX - 1 >= 0) {
                    const Tile* captureLeftTile = board.getTile(startX - 1, captureY);
                    if (captureLeftTile && captureLeftTile->getPiece() != PieceType::NONE &&
                        captureLeftTile->getPieceColor() != pieceColor) {
                        validMoves.push_back({startX - 1, captureY});
                    }
                }
                // Schlagen nach rechts
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

            for (int i = 1; i < 8-startY; ++i) {
                // Nach oben
                if (startY + i < 8) {
                    const Tile* targetTile = board.getTile(startX, startY + i);
                    if (targetTile) {
                        if (targetTile->getPiece() == PieceType::NONE) {
                            validMoves.push_back({startX, startY + i});
                        } else if (targetTile->getPieceColor() != pieceColor) {
                            validMoves.push_back({startX, startY + i});
                            break; // Blockiert durch eine gegnerische Figur
                        } else {
                            break; // Blockiert durch eine eigene Figur
                        }
                    }
                }
            }

            for (int i = 1; i < startY+1; ++i) {
                // Nach unten
                if (startY - i >= 0) {
                    const Tile* targetTile = board.getTile(startX, startY - i);
                    if (targetTile) {
                        if (targetTile->getPiece() == PieceType::NONE) {
                            validMoves.push_back({startX, startY - i});
                        } else if (targetTile->getPieceColor() != pieceColor) {
                            validMoves.push_back({startX, startY - i});
                            break; // Blockiert durch eine gegnerische Figur
                        } else {
                            break; // Blockiert durch eine eigene Figur
                        }
                    }
                }
            }

            for (int i = 1; i < startX+1; ++i) {
                // Nach links
                if (startX - i >= 0) {
                    const Tile* targetTile = board.getTile(startX - i, startY);
                    if (targetTile) {
                        if (targetTile->getPiece() == PieceType::NONE) {
                            validMoves.push_back({startX - i, startY});
                        } else if (targetTile->getPieceColor() != pieceColor) {
                            validMoves.push_back({startX - i, startY});
                            break; // Blockiert durch eine gegnerische Figur
                        } else {
                            break; // Blockiert durch eine eigene Figur
                        }
                    }
                }
            }

            for (int i = 1; i < 8-startX; ++i) {
                // Nach rechts
                if (startX + i < 8) {
                    const Tile* targetTile = board.getTile(startX + i, startY);
                    if (targetTile) {
                        if (targetTile->getPiece() == PieceType::NONE) {
                            validMoves.push_back({startX + i, startY});
                        } else if (targetTile->getPieceColor() != pieceColor) {
                            validMoves.push_back({startX + i, startY});
                            break; // Blockiert durch eine gegnerische Figur
                        } else {
                            break; // Blockiert durch eine eigene Figur
                        }
                    }
                }
            }

            break;
        case PieceType::KNIGHT:
            
            if(startX - 2 >= 0 && startY + 1 < 8) {
                const Tile* targetTile = board.getTile(startX - 2, startY + 1);
                if (targetTile && (targetTile->getPiece() == PieceType::NONE || targetTile->getPieceColor() != pieceColor)) {
                    validMoves.push_back({startX - 2, startY + 1});
                }
            }
            if(startX - 1 >= 0 && startY + 2 < 8) {
                const Tile* targetTile = board.getTile(startX - 1, startY + 2);
                if (targetTile && (targetTile->getPiece() == PieceType::NONE || targetTile->getPieceColor() != pieceColor)) {
                    validMoves.push_back({startX - 1, startY + 2});
                }
            }
            if(startX + 1 < 8 && startY + 2 < 8) {
                const Tile* targetTile = board.getTile(startX + 1, startY + 2);
                if (targetTile && (targetTile->getPiece() == PieceType::NONE || targetTile->getPieceColor() != pieceColor)) {
                    validMoves.push_back({startX + 1, startY + 2});
                }
            }
            if(startX + 2 < 8 && startY + 1 < 8) {
                const Tile* targetTile = board.getTile(startX + 2, startY + 1);
                if (targetTile && (targetTile->getPiece() == PieceType::NONE || targetTile->getPieceColor() != pieceColor)) {
                    validMoves.push_back({startX + 2, startY + 1});
                }
            }
            if(startX + 2 < 8 && startY - 1 >= 0) {
                const Tile* targetTile = board.getTile(startX + 2, startY - 1);
                if (targetTile && (targetTile->getPiece() == PieceType::NONE || targetTile->getPieceColor() != pieceColor)) {
                    validMoves.push_back({startX + 2, startY - 1});
                }
            }
            if(startX + 1 < 8 && startY - 2 >= 0) {
                const Tile* targetTile = board.getTile(startX + 1, startY - 2);
                if (targetTile && (targetTile->getPiece() == PieceType::NONE || targetTile->getPieceColor() != pieceColor)) {
                    validMoves.push_back({startX + 1, startY - 2});
                }
            }
            if(startX - 1 >= 0 && startY - 2 >= 0) {
                const Tile* targetTile = board.getTile(startX - 1, startY - 2);
                if (targetTile && (targetTile->getPiece() == PieceType::NONE || targetTile->getPieceColor() != pieceColor)) {
                    validMoves.push_back({startX - 1, startY - 2});
                }
            }
            if(startX - 2 >= 0 && startY - 1 >= 0) {
                const Tile* targetTile = board.getTile(startX - 2, startY - 1);
                if (targetTile && (targetTile->getPiece() == PieceType::NONE || targetTile->getPieceColor() != pieceColor)) {
                    validMoves.push_back({startX - 2, startY - 1});
                }
            }

            break;
        case PieceType::BISHOP:
            
            for (int i = 1; i < 8; ++i) {
                // Nach oben links
                if (startX - i >= 0 && startY + i < 8) {
                    const Tile* targetTile = board.getTile(startX - i, startY + i);
                    if (targetTile) {
                        if (targetTile->getPiece() == PieceType::NONE) {
                            validMoves.push_back({startX - i, startY + i});
                        } else if (targetTile->getPieceColor() != pieceColor) {
                            validMoves.push_back({startX - i, startY + i});
                            break; // Blockiert durch eine gegnerische Figur
                        } else {
                            break; // Blockiert durch eine eigene Figur
                        }
                    }
                }
            }

            for (int i = 1; i < 8; ++i) {
                // Nach oben rechts
                if (startX + i < 8 && startY + i < 8) {
                    const Tile* targetTile = board.getTile(startX + i, startY + i);
                    if (targetTile) {
                        if (targetTile->getPiece() == PieceType::NONE) {
                            validMoves.push_back({startX + i, startY + i});
                        } else if (targetTile->getPieceColor() != pieceColor) {
                            validMoves.push_back({startX + i, startY + i});
                            break; // Blockiert durch eine gegnerische Figur
                        } else {
                            break; // Blockiert durch eine eigene Figur
                        }
                    }
                }
            }

            for (int i = 1; i < 8; ++i) {
                // Nach unten links
                if (startX - i >= 0 && startY - i >= 0) {
                    const Tile* targetTile = board.getTile(startX - i, startY - i);
                    if (targetTile) {
                        if (targetTile->getPiece() == PieceType::NONE) {
                            validMoves.push_back({startX - i, startY - i});
                        } else if (targetTile->getPieceColor() != pieceColor) {
                            validMoves.push_back({startX - i, startY - i});
                            break; // Blockiert durch eine gegnerische Figur
                        } else {
                            break; // Blockiert durch eine eigene Figur
                        }
                    }
                }
            }

            for (int i = 1; i < 8; ++i) {
                // Nach unten rechts
                if (startX + i < 8 && startY - i >= 0) {
                    const Tile* targetTile = board.getTile(startX + i, startY - i);
                    if (targetTile) {
                        if (targetTile->getPiece() == PieceType::NONE) {
                            validMoves.push_back({startX + i, startY - i});
                        } else if (targetTile->getPieceColor() != pieceColor) {
                            validMoves.push_back({startX + i, startY - i});
                            break; // Blockiert durch eine gegnerische Figur
                        } else {
                            break; // Blockiert durch eine eigene Figur
                        }
                    }
                }
            }
        

            break;
        case PieceType::QUEEN:
            
            for (int i = 1; i < 8-startY; ++i) {
                // Nach oben
                if (startY + i < 8) {
                    const Tile* targetTile = board.getTile(startX, startY + i);
                    if (targetTile) {
                        if (targetTile->getPiece() == PieceType::NONE) {
                            validMoves.push_back({startX, startY + i});
                        } else if (targetTile->getPieceColor() != pieceColor) {
                            validMoves.push_back({startX, startY + i});
                            break; // Blockiert durch eine gegnerische Figur
                        } else {
                            break; // Blockiert durch eine eigene Figur
                        }
                    }
                }
            }

            for (int i = 1; i < startY+1; ++i) {
                // Nach unten
                if (startY - i >= 0) {
                    const Tile* targetTile = board.getTile(startX, startY - i);
                    if (targetTile) {
                        if (targetTile->getPiece() == PieceType::NONE) {
                            validMoves.push_back({startX, startY - i});
                        } else if (targetTile->getPieceColor() != pieceColor) {
                            validMoves.push_back({startX, startY - i});
                            break; // Blockiert durch eine gegnerische Figur
                        } else {
                            break; // Blockiert durch eine eigene Figur
                        }
                    }
                }
            }

            for (int i = 1; i < startX+1; ++i) {
                // Nach links
                if (startX - i >= 0) {
                    const Tile* targetTile = board.getTile(startX - i, startY);
                    if (targetTile) {
                        if (targetTile->getPiece() == PieceType::NONE) {
                            validMoves.push_back({startX - i, startY});
                        } else if (targetTile->getPieceColor() != pieceColor) {
                            validMoves.push_back({startX - i, startY});
                            break; // Blockiert durch eine gegnerische Figur
                        } else {
                            break; // Blockiert durch eine eigene Figur
                        }
                    }
                }
            }

            for (int i = 1; i < 8-startX; ++i) {
                // Nach rechts
                if (startX + i < 8) {
                    const Tile* targetTile = board.getTile(startX + i, startY);
                    if (targetTile) {
                        if (targetTile->getPiece() == PieceType::NONE) {
                            validMoves.push_back({startX + i, startY});
                        } else if (targetTile->getPieceColor() != pieceColor) {
                            validMoves.push_back({startX + i, startY});
                            break; // Blockiert durch eine gegnerische Figur
                        } else {
                            break; // Blockiert durch eine eigene Figur
                        }
                    }
                }
            }
            // Diagonale Bewegungen (wie Läufer)
            // Nach oben links
            for (int i = 1; i < 8; ++i) {
                // Nach oben links
                if (startX - i >= 0 && startY + i < 8) {
                    const Tile* targetTile = board.getTile(startX - i, startY + i);
                    if (targetTile) {
                        if (targetTile->getPiece() == PieceType::NONE) {
                            validMoves.push_back({startX - i, startY + i});
                        } else if (targetTile->getPieceColor() != pieceColor) {
                            validMoves.push_back({startX - i, startY + i});
                            break; // Blockiert durch eine gegnerische Figur
                        } else {
                            break; // Blockiert durch eine eigene Figur
                        }
                    }
                }
            }

            for (int i = 1; i < 8; ++i) {
                // Nach oben rechts
                if (startX + i < 8 && startY + i < 8) {
                    const Tile* targetTile = board.getTile(startX + i, startY + i);
                    if (targetTile) {
                        if (targetTile->getPiece() == PieceType::NONE) {
                            validMoves.push_back({startX + i, startY + i});
                        } else if (targetTile->getPieceColor() != pieceColor) {
                            validMoves.push_back({startX + i, startY + i});
                            break; // Blockiert durch eine gegnerische Figur
                        } else {
                            break; // Blockiert durch eine eigene Figur
                        }
                    }
                }
            }

            for (int i = 1; i < 8; ++i) {
                // Nach unten links
                if (startX - i >= 0 && startY - i >= 0) {
                    const Tile* targetTile = board.getTile(startX - i, startY - i);
                    if (targetTile) {
                        if (targetTile->getPiece() == PieceType::NONE) {
                            validMoves.push_back({startX - i, startY - i});
                        } else if (targetTile->getPieceColor() != pieceColor) {
                            validMoves.push_back({startX - i, startY - i});
                            break; // Blockiert durch eine gegnerische Figur
                        } else {
                            break; // Blockiert durch eine eigene Figur
                        }
                    }
                }
            }
            
            for (int i = 1; i < 8; ++i) {
                // Nach unten rechts
                if (startX + i < 8 && startY - i >= 0) {
                    const Tile* targetTile = board.getTile(startX + i, startY - i);
                    if (targetTile) {
                        if (targetTile->getPiece() == PieceType::NONE) {
                            validMoves.push_back({startX + i, startY - i});
                        } else if (targetTile->getPieceColor() != pieceColor) {
                            validMoves.push_back({startX + i, startY - i});
                            break; // Blockiert durch eine gegnerische Figur
                        } else {
                            break; // Blockiert durch eine eigene Figur
                        }
                    }
                }
            }

            break;
        case PieceType::KING:
            
            if(startY + 1 < 8) {
                const Tile* targetTile = board.getTile(startX, startY + 1);
                if (targetTile && (targetTile->getPiece() == PieceType::NONE || targetTile->getPieceColor() != pieceColor)) {
                    validMoves.push_back({startX, startY + 1});
                }
            }
            if(startY - 1 >= 0) {
                const Tile* targetTile = board.getTile(startX, startY - 1);
                if (targetTile && (targetTile->getPiece() == PieceType::NONE || targetTile->getPieceColor() != pieceColor)) {
                    validMoves.push_back({startX, startY - 1});
                }
            }
            if(startX - 1 >= 0 && startY + 1 < 8) {
                const Tile* targetTile = board.getTile(startX - 1, startY + 1);
                if (targetTile && (targetTile->getPiece() == PieceType::NONE || targetTile->getPieceColor() != pieceColor)) {
                    validMoves.push_back({startX - 1, startY + 1});
                }
            }
            if(startX + 1 < 8 && startY + 1 < 8) {
                const Tile* targetTile = board.getTile(startX + 1, startY + 1);
                if (targetTile && (targetTile->getPiece() == PieceType::NONE || targetTile->getPieceColor() != pieceColor)) {
                    validMoves.push_back({startX + 1, startY + 1});
                }
            }
            if(startX - 1 >= 0 && startY - 1 >= 0) {
                const Tile* targetTile = board.getTile(startX - 1, startY - 1);
                if (targetTile && (targetTile->getPiece() == PieceType::NONE || targetTile->getPieceColor() != pieceColor)) {
                    validMoves.push_back({startX - 1, startY - 1});
                }
            }
            if(startX + 1 < 8 && startY - 1 >= 0) {
                const Tile* targetTile = board.getTile(startX + 1, startY - 1);
                if (targetTile && (targetTile->getPiece() == PieceType::NONE || targetTile->getPieceColor() != pieceColor)) {
                    validMoves.push_back({startX + 1, startY - 1});
                }
            }
            if(startX - 1 >= 0) {
                const Tile* targetTile = board.getTile(startX - 1, startY);
                if (targetTile && (targetTile->getPiece() == PieceType::NONE || targetTile->getPieceColor() != pieceColor)) {
                    validMoves.push_back({startX - 1, startY});
                }
            }
            if(startX + 1 < 8) {
                const Tile* targetTile = board.getTile(startX + 1, startY);
                if (targetTile && (targetTile->getPiece() == PieceType::NONE || targetTile->getPieceColor() != pieceColor)) {
                    validMoves.push_back({startX + 1, startY});
                }
            }

            
            break;
        default:
            std::cerr << "Unbekannter Figurtyp: " << static_cast<int>(pieceType) << std::endl;
            break;
    }

    return validMoves;
}

}