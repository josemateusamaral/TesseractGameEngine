#pragma once
#include <iostream>

using namespace std;

class Texture
{
        
    public:

        unsigned char* data;
        int width;
        int height;

        Texture(unsigned char* data, int width, int height);
        Texture(string path);

};

ostream & operator<<(ostream &out, const Texture &p);