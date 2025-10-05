#include "Constants.hpp"
#include "PillarRenderer.hpp"

void PillarRenderer::Initialize()
{
    _texture = LoadTexture("assets/images/Block.png");
}

void PillarRenderer::Render(Pillar *pillars)
{
    for (int i = 0; i < TOTAL_PILLARS; i++)
    {
        auto pillar = pillars[i];

        for (float y = pillar.bottom.y; y < SCREEN_HEIGHT; y += 32)
        {
            DrawTexturePro(
                _texture,
                { 0, 0, 32, 32 },
                { pillar.bottom.x, y, 32, 32},
                { 0, 0 },
                0.0f,
                WHITE);
        }

        for (float y = pillar.top.y + pillar.top.height - 32; y > -32; y -= 32)
        {
            DrawTexturePro(
                _texture,
                { 0, 0, 32, 32 },
                { pillar.top.x, y, 32, 32},
                { 0, 0 },
                0.0f,
                WHITE);
        }
    }
}

void PillarRenderer::Uninitialize()
{
    UnloadTexture(_texture);
}
