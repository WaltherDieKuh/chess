#include "board.h"

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