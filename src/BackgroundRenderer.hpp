#pragma once
#include "Entities.hpp"
#include "raylib.h"

class BackgroundRenderer {
    Texture2D _texture;
public:
    void Initialize();
    void Render(Background &);
    void Unitialize();
};
