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

        for (int y = pillar.bottom.y; y < SCREEN_HEIGHT; y += 32)
        {
            DrawTexture(
                _texture,
                pillar.bottom.x,
                y,
                WHITE);
        }

        for (int y = pillar.top.y + pillar.top.height - 32; y > -32; y -= 32)
        {
            DrawTexture(
                _texture,
                pillar.top.x,
                y,
                WHITE);
        }
    }
}

void PillarRenderer::Uninitialize()
{
    UnloadTexture(_texture);
}
