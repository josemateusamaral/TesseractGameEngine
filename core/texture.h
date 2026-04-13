#pragma once
#include <iostream>

using namespace std;

class Texture
{
        
    public:

        char* data;
        int width;
        int height;

        Texture(char* data, int width, int height);

};

ostream & operator<<(ostream &out, const Texture &p);