#include "board.h"
#include "mouse_handler.h"
#include <iostream>

ChessBoard::ChessBoard() {
    board.resize(8);
    for (int i = 0; i < 8; ++i) {
        board[i].resize(8);
    }
    initializeBoard();
}

void ChessBoard::initializeBoard() {
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            TileColor color = ((x + y) % 2 == 0) ? TileColor::WHITE : TileColor::BLACK;
            board[x][y] = std::make_unique<Tile>(x, y, color);
        }
    }
}

void ChessBoard::setupFromFEN(const std::string& fen, PieceManager& pieceManager) {
    currentFEN = fen;
    
    // Alle Figuren entfernen
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            board[x][y]->removePiece();
        }
    }
    
    // FEN parsen und Figuren setzen
    auto positions = fenParser.parseFEN(fen);
    
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            const auto& pos = positions[x][y];
            if (!pos.isEmpty) {
                auto texture = pieceManager.getTexture(pos.piece, pos.color);
                if (texture) {
                    board[x][y]->setPiece(pos.piece, pos.color, texture);
                }
            }
        }
    }
}

void ChessBoard::setupInitialPieces(PieceManager& pieceManager) {
    // Standard-Schachposition als FEN
    std::string startFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    setupFromFEN(startFEN, pieceManager);
}

void ChessBoard::setPosition(const std::string& fen, PieceManager& pieceManager) {
    setupFromFEN(fen, pieceManager);
}

void ChessBoard::drawBoard(unsigned int shaderProgram, unsigned int VAO) const {
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            board[x][y]->drawTile(shaderProgram, VAO);
        }
    }
}

void ChessBoard::drawBoard(unsigned int shaderProgram, unsigned int VAO, const MouseHandler& mouseHandler) const {
    auto [selectedX, selectedY] = mouseHandler.getSelected();
    auto [hoveredX, hoveredY] = mouseHandler.getHovered();
    const auto& validMoves = mouseHandler.getValidMoveHighlights(); // Hole die Liste der gültigen Züge
    
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            bool isSelectedTile = (x == selectedX && y == selectedY);
            bool isHoveredTile = (x == hoveredX && y == hoveredY);
            
            bool isPossibleMoveTarget = false;
            if (selectedX != -1) { // Nur wenn eine Figur ausgewählt ist, zeige mögliche Züge
                for (const auto& move : validMoves) {
                    if (move.first == x && move.second == y) {
                        isPossibleMoveTarget = true;
                        break;
                    }
                }
            }
            
            board[x][y]->drawTile(shaderProgram, VAO, isSelectedTile, isHoveredTile, isPossibleMoveTarget);
        }
    }
}

Tile* ChessBoard::getTile(int x, int y) const {
    if (x >= 0 && x < 8 && y >= 0 && y < 8) {
        return board[x][y].get();
    }
    return nullptr;
}