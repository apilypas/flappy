#include "LabelRenderer.hpp"

void LabelRenderer::Initialize()
{
    _font = GetFontDefault();
}

void LabelRenderer::Render(Label &label)
{
    if (label.isVisible)
    {
        float shadowOffset = label.fontSize / 20.0f;

        DrawTextPro(
            _font,
            label.text,
            (Vector2) { label.x + shadowOffset, label.y + shadowOffset },
            (Vector2) { 0, 0 },
            label.rotation,
            label.fontSize,
            2,
            label.shadowColor);

        DrawTextPro(
            _font,
            label.text,
            (Vector2) { (float)label.x , (float)label.y },
            (Vector2) { 0, 0 },
            label.rotation,
            label.fontSize,
            2,
            label.color);
    }
}

void LabelRenderer::Uninitialize()
{
    UnloadFont(_font);
}
