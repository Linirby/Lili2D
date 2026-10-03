#pragma once

namespace lili {

class Render2DSystem
{
public:
    void
    render()
    {
        renderSprites();
    }

private:
    void
    renderSprites();
};

void
Render2DSystem::renderSprites()
{
}

} // namespace lili
