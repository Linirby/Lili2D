#pragma once

#include <lili2d/lili2d.hpp>

class MainScene : public lili::Scene {
public:
    explicit MainScene(lili::Renderer* renderer);

    void
    handleEvents(const lili::Event& event) override;
    void
    render(float alpha) override;

private:
    lili::Text text;
    lili::Text info_text;
};
