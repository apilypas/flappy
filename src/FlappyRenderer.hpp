#pragma once
#include "Entities.hpp"

class FlappyRenderer {
    Texture2D _texture;
public:
    void Initialize();
    void Render(Flappy&);
    void Uninitialize();
};
