#include "rng_helper.hpp"

#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec4.hpp"
#include <random>

std::mt19937&
rng::gen()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen;
}

lili::Vec2
rng::pos()
{
    std::mt19937& gen = rng::gen();
    std::uniform_real_distribution<float> posX(0.0f, 800.0f);
    std::uniform_real_distribution<float> posY(0.0f, 600.0f);

    return { posX(gen), posY(gen) };
}

lili::Vec2
rng::size()
{
    std::mt19937& gen = rng::gen();
    std::uniform_real_distribution<float> width(10.0f, 50.0f);
    std::uniform_real_distribution<float> height(10.0f, 50.0f);

    return { width(gen), height(gen) };
}

float
rng::radius()
{
    std::mt19937& gen = rng::gen();
    std::uniform_real_distribution<float> res(5.0f, 25.0f);

    return res(gen);
}

lili::Vec4
rng::color()
{
    std::mt19937& gen = rng::gen();
    std::uniform_real_distribution<float> red(0.0f, 1.0f);
    std::uniform_real_distribution<float> green(0.0f, 1.0f);
    std::uniform_real_distribution<float> blue(0.0f, 1.0f);

    return { red(gen), green(gen), blue(gen), 1.0f };
}

bool
rng::hollow()
{
    std::mt19937& gen = rng::gen();
    std::uniform_int_distribution<> res(0, 1);

    return res(gen);
}
