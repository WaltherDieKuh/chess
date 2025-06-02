#include "board.h"
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

Tile* ChessBoard::getTile(int x, int y) const {
    if (x >= 0 && x < 8 && y >= 0 && y < 8) {
        return board[x][y].get();
    }
    return nullptr;
}