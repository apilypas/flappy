#include <cmath>
#include "LabelRenderer.hpp"

void LabelRenderer::Initialize()
{
    _font = LoadFontEx("assets/fonts/golem-script.ttf", 64, 0, 0);
}

void LabelRenderer::Render(Label &label)
{
    if (label.isVisible)
    {
        float shadowOffset = ceilf(label.fontSize / 20.0f);
        float spacing = ceilf(label.fontSize / 10.0f);

        DrawTextPro(
            _font,
            label.text,
            (Vector2) { label.x + shadowOffset, label.y + shadowOffset },
            (Vector2) { 0, 0 },
            label.rotation,
            label.fontSize,
            spacing,
            label.shadowColor);

        DrawTextPro(
            _font,
            label.text,
            (Vector2) { (float)label.x , (float)label.y },
            (Vector2) { 0, 0 },
            label.rotation,
            label.fontSize,
            spacing,
            label.color);
    }
}

void LabelRenderer::Uninitialize()
{
    UnloadFont(_font);
}
