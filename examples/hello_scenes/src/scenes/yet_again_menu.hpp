#pragma once

#include <lili2d/lili2d.hpp>

class YetAgainScene : public lili::Scene {
public:
    explicit YetAgainScene(lili::Renderer* renderer);

    void
    handleEvents(const lili::Event& event) override;
    void
    render(float alpha) override;

private:
    lili::Text text;
    lili::Text info_text;
};
