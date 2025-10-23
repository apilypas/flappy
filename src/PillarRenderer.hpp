#pragma once
#include "Entities.hpp"
#include <raylib.h>

class PillarRenderer
{
    Texture2D _texture;

public:
    void Initialize();
    void Render(Pillar &);
    void Uninitialize();
};
