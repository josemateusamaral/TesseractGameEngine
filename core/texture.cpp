#include "texture.h"

Texture::Texture(char* data, int width, int height)
    : data(data), width(width), height(height) {}

ostream & operator<<(ostream &out, const Texture &p) {
    out << "Texture: " << p.width << "x" << p.height;
    return out;
}
