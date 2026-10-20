#include "rng_helper.hpp"

using namespace lili;

std::mt19937&
rng::gen()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen;
}

Vec2
rng::pos(Vec2 screen_size)
{
    std::mt19937& gen = rng::gen();
    std::uniform_real_distribution<float> posX(0.0f, screen_size.x);
    std::uniform_real_distribution<float> posY(0.0f, screen_size.y);

    return { posX(gen), posY(gen) };
}

Vec2
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

Vec4
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

float
rng::thickness()
{
    std::mt19937& gen = rng::gen();
    std::uniform_real_distribution<float> res(3.0f, 9.0f);

    return res(gen);
}
