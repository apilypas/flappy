#include "GameBannerRenderer.hpp"
#include "Constants.hpp"
#include "Entities.hpp"
#include <raylib.h>

void GameBannerRenderer::Initialize()
{
    _font = LoadFontEx("assets/fonts/golem-script.ttf",  (int)_textFontSize * 4, 0, 0);
}

void GameBannerRenderer::Render(GameBanner &b)
{
    if (!b.isVisible) return;

    float screenWidth = (float)GetRenderWidth();
    float screenHeight = (float)GetRenderHeight();

    float width = (SCREEN_WIDTH - 64.0f) * b.scale;
    float height = (SCREEN_HEIGHT - 64.0f) * b.scale;
    float x = (screenWidth - width) / 2.0f;
    float y = (screenHeight - height) / 2.0f;
    Rectangle box ={ x, y, width, height };

    DrawRectanglePro(
        box,
        { 0, 0 },
        0.0f,
        { 0x00, 0x00, 0x00, 0xAA }
    );

    this->DrawText(
        b.bannerText,
        80 * b.scale,
        box,
        { 0, -100.0f * b.scale },
        { 0xFF, 0xC9, 0x00, 0xFF });
    
    this->DrawText(
        b.authorText,
        20 * b.scale,
        box,
        { 0, 40.0f * b.scale },
        WHITE);

    this->DrawText(
        b.text,
        _textFontSize * b.scale,
        box,
        { 0, 250.0f * b.scale },
        b.color);
}

void GameBannerRenderer::Uninitialize()
{
    UnloadFont(_font);
}

void GameBannerRenderer::DrawText(const char *text, float fontSize, Rectangle box, Vector2 pos, Color color)
{
    float shadowOffset = fontSize / 20.0f;
    float spacing = fontSize / 10.0f;

    auto size = MeasureTextEx(
        _font,
        text,
        fontSize,
        spacing);
        
    float textX = box.x + (box.width - size.x + pos.x) / 2.0f;
    float texty = box.y + (box.height - size.y + pos.y) / 2.0f;

    DrawTextPro(
        _font,
        text,
        { textX + shadowOffset, texty + shadowOffset },
        { 0, 0 },
        0,
        fontSize,
        spacing,
        BLACK);

    DrawTextPro(
        _font,
        text,
        { textX , texty },
        { 0, 0 },
        0,
        fontSize,
        spacing,
        color);
}
