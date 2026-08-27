#pragma once

#include <cmath>

namespace lili {

/// @brief Easing function types for interpolation curves.
enum class EaseType {
    LINEAR,

    IN_QUAD,
    OUT_QUAD,
    IN_OUT_QUAD,

    IN_CUBIC,
    OUT_CUBIC,
    IN_OUT_CUBIC,

    IN_SINE,
    OUT_SINE,
    IN_OUT_SINE,

    IN_EXPO,
    OUT_EXPO,
    IN_OUT_EXPO,

    IN_BACK,
    OUT_BACK,
    IN_OUT_BACK,

    IN_ELASTIC,
    OUT_ELASTIC,
    IN_OUT_ELASTIC,

    IN_BOUNCE,
    OUT_BOUNCE,
    IN_OUT_BOUNCE
};

namespace detail {
constexpr float PI = 3.14159265358979323846f;
}

/// @brief Provides mathematical interpolation and easing functions.
class Easing {
public:
    /// @brief Linear interpolation (constant speed).
    [[nodiscard]] static constexpr float
    linear(float t) noexcept {
        return t;
    }

    /// @brief Quadratic ease-in (accelerating).
    [[nodiscard]] static constexpr float
    inQuad(float t) noexcept {
        return t * t;
    }

    /// @brief Quadratic ease-out (decelerating).
    [[nodiscard]] static constexpr float
    outQuad(float t) noexcept {
        float inv = 1.0f - t;
        return 1.0f - inv * inv;
    }

    /// @brief Quadratic ease-in-out (acceleration then deceleration).
    [[nodiscard]] static constexpr float
    inOutQuad(float t) noexcept {
        float inv = 1.0f - t;
        return t < 0.5f ? 2.0f * t * t : 1.0f - 2.0f * inv * inv;
    }

    /// @brief Cubic ease-in.
    [[nodiscard]] static constexpr float
    inCubic(float t) noexcept {
        return t * t * t;
    }

    /// @brief Cubic ease-out.
    [[nodiscard]] static constexpr float
    outCubic(float t) noexcept {
        float inv = 1.0f - t;
        return 1.0f - inv * inv * inv;
    }

    /// @brief Cubic ease-in-out.
    [[nodiscard]] static constexpr float
    inOutCubic(float t) noexcept {
        float inv = 1.0f - t;
        return t < 0.5f ? 4.0f * t * t * t : 1.0f - 4.0f * inv * inv * inv;
    }

    /// @brief Sinusoidal ease-in.
    [[nodiscard]] static inline float
    inSine(float t) noexcept {
        return 1.0f - std::cos(t * (detail::PI * 0.5f));
    }

    /// @brief Sinusoidal ease-out.
    [[nodiscard]] static inline float
    outSine(float t) noexcept {
        return std::sin(t * (detail::PI * 0.5f));
    }

    /// @brief Sinusoidal ease-in-out.
    [[nodiscard]] static inline float
    inOutSine(float t) noexcept {
        return 0.5f * (1.0f - std::cos(detail::PI * t));
    }

    /// @brief Exponential ease-in.
    [[nodiscard]] static inline float
    inExpo(float t) noexcept {
        return std::exp2(10.0f * (t - 1.0f));
    }

    /// @brief Exponential ease-out.
    [[nodiscard]] static inline float
    outExpo(float t) noexcept {
        return 1.0f - std::exp2(-10.0f * t);
    }

    /// @brief Exponential ease-in-out.
    [[nodiscard]] static inline float
    inOutExpo(float t) noexcept {
        return t < 0.5f ? 0.5f * std::exp2(20.0f * t - 10.0f)
                        : 1.0f - 0.5f * std::exp2(-20.0f * t + 10.0f);
    }

    /// @brief Back ease-in (overshoots behind before moving forward).
    [[nodiscard]] static constexpr float
    inBack(float t) noexcept {
        constexpr float c1 = 1.70158f;
        constexpr float c3 = c1 + 1.0f;
        return t * t * (c3 * t - c1);
    }

    /// @brief Back ease-out (overshoots past destination then settles).
    [[nodiscard]] static constexpr float
    outBack(float t) noexcept {
        constexpr float c1 = 1.70158f;
        constexpr float c3 = c1 + 1.0f;
        float inv = t - 1.0f;
        return 1.0f + inv * inv * (c3 * inv + c1);
    }

    /// @brief Back ease-in-out.
    [[nodiscard]] static constexpr float
    inOutBack(float t) noexcept {
        constexpr float c1 = 1.70158f;
        constexpr float c2 = c1 * 1.525f;
        if (t < 0.5f) return 2.0f * t * t * ((c2 + 1.0f) * 2.0f * t - c2);
        float inv = 2.0f * t - 2.0f;
        return 0.5f * (inv * inv * ((c2 + 1.0f) * inv + c2) + 2.0f);
    }

    /// @brief Elastic ease-in (oscillating acceleration).
    [[nodiscard]] static inline float
    inElastic(float t) noexcept {
        constexpr float c4 = (2.0f * detail::PI) / 3.0f;
        return -std::exp2(10.0f * (t - 1.0f)) *
               std::sin((10.0f * t - 10.75f) * c4);
    }

    /// @brief Elastic ease-out (oscillating deceleration).
    [[nodiscard]] static inline float
    outElastic(float t) noexcept {
        constexpr float c4 = (2.0f * detail::PI) / 3.0f;
        return std::exp2(-10.0f * t) * std::sin((10.0f * t - 0.75f) * c4) +
               1.0f;
    }

    /// @brief Elastic ease-in-out.
    [[nodiscard]] static inline float
    inOutElastic(float t) noexcept {
        constexpr float c5 = (2.0f * detail::PI) / 4.5f;
        float s = std::sin((20.0f * t - 11.125f) * c5);
        return t < 0.5f ? -0.5f * std::exp2(20.0f * t - 10.0f) * s
                        : 0.5f * std::exp2(-20.0f * t + 10.0f) * s + 1.0f;
    }

    /// @brief Bounce ease-out (bouncing ball deceleration).
    [[nodiscard]] static constexpr float
    outBounce(float t) noexcept {
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

    /// @brief Bounce ease-in (bouncing ball acceleration).
    [[nodiscard]] static constexpr float
    inBounce(float t) noexcept {
        return 1.0f - outBounce(1.0f - t);
    }

    /// @brief Bounce ease-in-out.
    [[nodiscard]] static constexpr float
    inOutBounce(float t) noexcept {
        return t < 0.5f ? 0.5f * (1.0f - outBounce(1.0f - 2.0f * t))
                        : 0.5f * (1.0f + outBounce(2.0f * t - 1.0f));
    }

    /// @brief Evaluates an easing curve dynamically by EaseType.
    /// @param type Easing curve type.
    /// @param t Normalized time progress in range [0, 1].
    /// @return Interpolated progress value.
    [[nodiscard]] static inline float
    evaluate(EaseType type, float t) noexcept {
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
};

}  // namespace lili
