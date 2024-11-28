#include "TextureLoader.h"

TextureLoader::TextureLoader() { }

TextureLoader::~TextureLoader() {
    clean_up();
}

void TextureLoader::clean_up() {
    for (auto& [key, file] : textures)
    {
        delete file;
    }
    textures.clear();
}
