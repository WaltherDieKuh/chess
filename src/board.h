#pragma once
#include "tile.h"
#include <vector>
#include <memory>

class ChessBoard {
private:
    std::vector<std::vector<std::unique_ptr<Tile>>> board;

public:
    ChessBoard();
    void initializeBoard();
    void drawBoard(unsigned int shaderProgram, unsigned int VAO) const;
    Tile* getTile(int x, int y) const;
};