#pragma once

#include "Entities.hpp"
#include <raylib.h>

class GameBannerRenderer
{
    const float _textFontSize = 40.0f;
    Font _font;

public:
    void Initialize();
    void Render(GameBanner &);
    void Uninitialize();

private:
    void DrawText(const char *, float, Rectangle, Vector2, Color);
};
