#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "vendor/stb_image.h"

Texture::Texture(unsigned char* data, int width, int height)
    : data(data), width(width), height(height) {}

Texture::Texture(string path){
    int width, height, nrChannels;
    unsigned char* data = nullptr;
    string texPath = path;
    data = stbi_load(texPath.c_str(), &width, &height, &nrChannels, 3);
    if (data) {
        this->data = data;
        this->height = height;
        this->width = width;
        printf("Sucesso: Textura carregada (%dx%d)\n", width, height);
    } else {
        printf("Erro ao carregar textura: %s\n", stbi_failure_reason());
    }
}

ostream & operator<<(ostream &out, const Texture &p) {
    out << "Texture: " << p.width << "x" << p.height;
    return out;
}
