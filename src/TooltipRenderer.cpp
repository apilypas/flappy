#include <raylib.h>
#include "TooltipRenderer.hpp"

void TooltipRenderer::Initialize()
{
    _font = LoadFontEx("assets/fonts/golem-script.ttf",  24, 0, 0);
}

void TooltipRenderer::Render(Tooltip &tooltip)
{
    auto size = MeasureTextEx(_font, tooltip.text, 14, 1.0f);
    float x = tooltip.x - (size.x / 2.0f);
    float y = tooltip.y - (size.y / 2.0f);

    DrawRectangleRounded(
        { x - 3.0f , y + 1.0f, size.x + 8.0f, size.y + 8.0f },
        2.0f,
        6,
        { 0x00, 0x00, 0x00, 0x66 });

    DrawTextPro(
        _font,
        tooltip.text,
        { x + 1.0f, tooltip.y + 1.0f },
        { 0, 0 },
        0.0f,
        14.0f,
        1.0f,
        { 0x00, 0x00, 0x00, 0x66 });

    DrawTextPro(
        _font,
        tooltip.text,
        { x , tooltip.y },
        { 0, 0 },
        0.0f,
        14.0f,
        1.0f,
        { 0xFF, 0xFF, 0xFF, 0x66 });
}

void TooltipRenderer::Uninitialize()
{
    UnloadFont(_font);
}
