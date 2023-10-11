#pragma once

#include "Color.h"
#include "Common.h"

namespace MiniFPS
{
    struct Texture
    {
        string name;
        Color** buffer;
        int size;

        Texture();

        Texture(const string& name, const string& filePath);
    };
}