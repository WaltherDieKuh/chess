#include "piece_manager.h"
#include <vector>
#include <iostream> // Für Debug-Ausgaben

bool PieceManager::loadTextures(const std::string& assetPath) {
    std::vector<std::pair<std::pair<PieceType, PieceColor>, std::string>> textureFiles = {
        {{PieceType::PAWN, PieceColor::WHITE}, assetPath + "/pawn_white.png"},
        {{PieceType::ROOK, PieceColor::WHITE}, assetPath + "/rook_white.png"},
        {{PieceType::KNIGHT, PieceColor::WHITE}, assetPath + "/knight_white.png"},
        {{PieceType::BISHOP, PieceColor::WHITE}, assetPath + "/bishop_white.png"},
        {{PieceType::QUEEN, PieceColor::WHITE}, assetPath + "/queen_white.png"},
        {{PieceType::KING, PieceColor::WHITE}, assetPath + "/king_white.png"},
        {{PieceType::PAWN, PieceColor::BLACK}, assetPath + "/pawn_black.png"},
        {{PieceType::ROOK, PieceColor::BLACK}, assetPath + "/rook_black.png"},
        {{PieceType::KNIGHT, PieceColor::BLACK}, assetPath + "/knight_black.png"},
        {{PieceType::BISHOP, PieceColor::BLACK}, assetPath + "/bishop_black.png"},
        {{PieceType::QUEEN, PieceColor::BLACK}, assetPath + "/queen_black.png"},
        {{PieceType::KING, PieceColor::BLACK}, assetPath + "/king_black.png"}
    };
    
    for (auto& [key, filepath] : textureFiles) {
        std::cout << "Trying to load: " << filepath << std::endl;  // Debug-Ausgabe
        auto texture = std::make_shared<Texture>();
        if (texture->loadFromFile(filepath)) {
            textures[key] = texture;
            std::cout << "Successfully loaded: " << filepath << std::endl;
        } else {
            std::cout << "Failed to load: " << filepath << std::endl;
            return false;
        }
    }
    return true;
}

std::shared_ptr<Texture> PieceManager::getTexture(PieceType type, PieceColor color) {
    auto key = std::make_pair(type, color);
    auto it = textures.find(key);
    return (it != textures.end()) ? it->second : nullptr;
}