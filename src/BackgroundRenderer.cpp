#include "BackgroundRenderer.hpp"

void BackgroundRenderer::Initialize()
{
    _texture = LoadTexture("assets/images/Sky.png");
}

void BackgroundRenderer::Render(Background &background)
{
    for (float i = -8; i < 18; i++) {
        for (float j = -6; j < 18; j++) {
            DrawTexturePro(_texture,
                           {0, 0, 128, 128},
                           {background.parallaxX + (float)i * 128.0f,
                            (float)j * 128.0f,
                            128,
                            128},
                           {0, 0},
                           0.0f,
                           WHITE);
        }
    }
}

void BackgroundRenderer::Uninitialize() { UnloadTexture(_texture); }
