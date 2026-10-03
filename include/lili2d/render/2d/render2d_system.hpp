#pragma once

namespace lili {

class Render2DSystem
{
public:
    static void
    render()
    {
        renderSprites();
    }

private:
    static void
    renderSprites();
};

void
Render2DSystem::renderSprites()
{
}

} // namespace lili
