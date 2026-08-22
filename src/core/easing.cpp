#include "lili2d/core/easing.hpp"

#include <cmath>

namespace lili {

namespace {

constexpr float PI = 3.14159265358979323846f;

}  // namespace

float
Easing::evaluate(EaseType type, float t) noexcept {
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;

    switch (type) {
        case EaseType::LINEAR:
            return linear(t);

        case EaseType::IN_QUAD:
            return inQuad(t);
        case EaseType::OUT_QUAD:
            return outQuad(t);
        case EaseType::IN_OUT_QUAD:
            return inOutQuad(t);

        case EaseType::IN_CUBIC:
            return inCubic(t);
        case EaseType::OUT_CUBIC:
            return outCubic(t);
        case EaseType::IN_OUT_CUBIC:
            return inOutCubic(t);

        case EaseType::IN_SINE:
            return inSine(t);
        case EaseType::OUT_SINE:
            return outSine(t);
        case EaseType::IN_OUT_SINE:
            return inOutSine(t);

        case EaseType::IN_EXPO:
            return inExpo(t);
        case EaseType::OUT_EXPO:
            return outExpo(t);
        case EaseType::IN_OUT_EXPO:
            return inOutExpo(t);

        case EaseType::IN_BACK:
            return inBack(t);
        case EaseType::OUT_BACK:
            return outBack(t);
        case EaseType::IN_OUT_BACK:
            return inOutBack(t);

        case EaseType::IN_ELASTIC:
            return inElastic(t);
        case EaseType::OUT_ELASTIC:
            return outElastic(t);
        case EaseType::IN_OUT_ELASTIC:
            return inOutElastic(t);

        case EaseType::OUT_BOUNCE:
            return outBounce(t);
        case EaseType::IN_BOUNCE:
            return inBounce(t);
        case EaseType::IN_OUT_BOUNCE:
            return inOutBounce(t);

        default:
            return t;
    }
}

float
Easing::linear(float t) noexcept {
    return t;
}

float
Easing::inQuad(float t) noexcept {
    return t * t;
}

float
Easing::outQuad(float t) noexcept {
    float inv = 1.0f - t;
    return 1.0f - inv * inv;
}

float
Easing::inOutQuad(float t) noexcept {
    float inv = 1.0f - t;
    return t < 0.5f ? 2.0f * t * t : 1.0f - 2.0f * inv * inv;
}

float
Easing::inCubic(float t) noexcept {
    return t * t * t;
}

float
Easing::outCubic(float t) noexcept {
    float inv = 1.0f - t;
    return 1.0f - inv * inv * inv;
}

float
Easing::inOutCubic(float t) noexcept {
    float inv = 1.0f - t;
    return t < 0.5f ? 4.0f * t * t * t : 1.0f - 4.0f * inv * inv * inv;
}

float
Easing::inSine(float t) noexcept {
    return 1.0f - std::cos(t * (PI * 0.5f));
}

float
Easing::outSine(float t) noexcept {
    return std::sin(t * (PI * 0.5f));
}

float
Easing::inOutSine(float t) noexcept {
    return 0.5f * (1.0f - std::cos(PI * t));
}

float
Easing::inExpo(float t) noexcept {
    return std::exp2(10.0f * (t - 1.0f));
}

float
Easing::outExpo(float t) noexcept {
    return 1.0f - std::exp2(-10.0f * t);
}

float
Easing::inOutExpo(float t) noexcept {
    return t < 0.5f ? 0.5f * std::exp2(20.0f * t - 10.0f)
                    : 1.0f - 0.5f * std::exp2(-20.0f * t + 10.0f);
}

float
Easing::inBack(float t) noexcept {
    constexpr float c1 = 1.70158f;
    constexpr float c3 = c1 + 1.0f;
    return t * t * (c3 * t - c1);
}

float
Easing::outBack(float t) noexcept {
    constexpr float c1 = 1.70158f;
    constexpr float c3 = c1 + 1.0f;
    float inv = t - 1.0f;
    return 1.0f + inv * inv * (c3 * inv + c1);
}

float
Easing::inOutBack(float t) noexcept {
    constexpr float c1 = 1.70158f;
    constexpr float c2 = c1 * 1.525f;
    if (t < 0.5f) return 2.0f * t * t * ((c2 + 1.0f) * 2.0f * t - c2);
    float inv = 2.0f * t - 2.0f;
    return 0.5f * (inv * inv * ((c2 + 1.0f) * inv + c2) + 2.0f);
}

float
Easing::inElastic(float t) noexcept {
    constexpr float c4 = (2.0f * PI) / 3.0f;
    return -std::exp2(10.0f * (t - 1.0f)) * std::sin((10.0f * t - 10.75f) * c4);
}

float
Easing::outElastic(float t) noexcept {
    constexpr float c4 = (2.0f * PI) / 3.0f;
    return std::exp2(-10.0f * t) * std::sin((10.0f * t - 0.75f) * c4) + 1.0f;
}

float
Easing::inOutElastic(float t) noexcept {
    constexpr float c5 = (2.0f * PI) / 4.5f;
    float s = std::sin((20.0f * t - 11.125f) * c5);
    return t < 0.5f ? -0.5f * std::exp2(20.0f * t - 10.0f) * s
                    : 0.5f * std::exp2(-20.0f * t + 10.0f) * s + 1.0f;
}

float
Easing::outBounce(float t) noexcept {
    constexpr float n1 = 7.5625f;
    constexpr float d1 = 2.75f;
    if (t < 1.0f / d1) return n1 * t * t;
    if (t < 2.0f / d1) {
        float sub_t = t - 1.5f / d1;
        return n1 * sub_t * sub_t + 0.75f;
    }
    if (t < 2.5f / d1) {
        float sub_t = t - 2.25f / d1;
        return n1 * sub_t * sub_t + 0.9375f;
    }
    float sub_t = t - 2.625f / d1;
    return n1 * sub_t * sub_t + 0.984375f;
}

float
Easing::inBounce(float t) noexcept {
    return 1.0f - outBounce(1.0f - t);
}

float
Easing::inOutBounce(float t) noexcept {
    return t < 0.5f ? 0.5f * (1.0f - outBounce(1.0f - 2.0f * t))
                    : 0.5f * (1.0f + outBounce(2.0f * t - 1.0f));
}

}  // namespace lili
