#pragma once
#include "Entities.hpp"
#include <raylib.h>

class PowerUpRenderer
{
    Texture2D _texture;

public:
    void Initialize();
    void Render(PowerUp &);
    void Uninitialize();
};
