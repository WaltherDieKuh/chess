#pragma once
#include "texture.h"
#include "tile.h"
#include <map>
#include <memory>
#include <string>

class PieceManager {
private:
    std::map<std::pair<PieceType, PieceColor>, std::shared_ptr<Texture>> textures;

public:
    bool loadTextures(const std::string& assetPath);
    std::shared_ptr<Texture> getTexture(PieceType type, PieceColor color);
};