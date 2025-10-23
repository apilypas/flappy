#include "PillarRenderer.hpp"
#include <raylib.h>

void PillarRenderer::Initialize()
{
    _texture = LoadTexture("assets/images/Block.png");
}

void PillarRenderer::Render(Pillar &pillar)
{
    DrawTexturePro(_texture,
                   {16, 16, 32, 32},
                   {pillar.bottom.x, pillar.bottom.y, 32, 32},
                   {0, 0},
                   0.0f,
                   WHITE);

    for (float y = pillar.bottom.y + 32; y < pillar.bottom.height; y += 32) {
        DrawTexturePro(_texture,
                       {16, 48, 32, 32},
                       {pillar.bottom.x, y, 32, 32},
                       {0, 0},
                       0.0f,
                       WHITE);
    }

    DrawTexturePro(
        _texture,
        {16, 80, 32, 32},
        {pillar.top.x, pillar.top.y + pillar.top.height - 32, 32, 32},
        {0, 0},
        0.0f,
        WHITE);

    for (float y = pillar.top.y; y < pillar.top.y + pillar.top.height - 32;
         y += 32) {
        DrawTexturePro(_texture,
                       {16, 48, 32, 32},
                       {pillar.top.x, y, 32, 32},
                       {0, 0},
                       0.0f,
                       WHITE);
    }

    if (pillar.door.width > 0.01f && pillar.door.height > 0.01f) {
        if (pillar.isLocked) {
            for (float y = pillar.door.y;
                 y < pillar.door.y + pillar.door.height - 0.01f;
                 y += 32) {
                DrawTexturePro(_texture,
                               {80, 16, 32, 32},
                               {pillar.door.x, y, 32, 32},
                               {0, 0},
                               0.0f,
                               WHITE);
            }

            DrawTexturePro(_texture,
                           {80, 80, 32, 32},
                           {pillar.lockCenter.x - 16.0f,
                            pillar.lockCenter.y - 16.0f,
                            32,
                            32},
                           {0, 0},
                           0.0f,
                           WHITE);
        } else {
            DrawTexturePro(_texture,
                           {80, 48, 32, 32},
                           {pillar.lockCenter.x - 16.0f,
                            pillar.lockCenter.y - 16.0f,
                            32,
                            32},
                           {0, 0},
                           0.0f,
                           WHITE);
        }
    }
}

void PillarRenderer::Uninitialize() { UnloadTexture(_texture); }
