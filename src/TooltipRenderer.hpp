#pragma once

#include "Entities.hpp"

class TooltipRenderer
{
    Font _font;

public:
    void Initialize();
    void Render(Tooltip &);
    void Uninitialize();
};
