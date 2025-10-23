#pragma once
#include "Entities.hpp"
#include "raylib.h"

class LabelRenderer
{
    Font _font;

public:
    void Initialize();
    void Render(Label &);
    void Uninitialize();
};