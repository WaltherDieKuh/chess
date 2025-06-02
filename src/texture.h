#pragma once
#include <glad/glad.h>
#include <string>

class Texture {
private:
    unsigned int textureID;
    int width, height, channels;

public:
    Texture();
    ~Texture();
    
    bool loadFromFile(const std::string& filepath);
    void bind(unsigned int slot = 0) const;
    void unbind() const;
    
    unsigned int getID() const { return textureID; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};