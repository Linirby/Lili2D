#pragma once

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

/// @brief Provides mathematical interpolation and easing functions.
class Easing {
public:
    /// @brief Evaluates an easing curve dynamically by EaseType.
    /// @param type Easing curve type.
    /// @param t Normalized time progress in range [0, 1].
    /// @return Interpolated progress value.
    static float
    evaluate(EaseType type, float t) noexcept;

    /// @brief Linear interpolation (constant speed).
    static float
    linear(float t) noexcept;

    /// @brief Quadratic ease-in (accelerating).
    static float
    inQuad(float t) noexcept;
    /// @brief Quadratic ease-out (decelerating).
    static float
    outQuad(float t) noexcept;
    /// @brief Quadratic ease-in-out (acceleration then deceleration).
    static float
    inOutQuad(float t) noexcept;

    /// @brief Cubic ease-in.
    static float
    inCubic(float t) noexcept;
    /// @brief Cubic ease-out.
    static float
    outCubic(float t) noexcept;
    /// @brief Cubic ease-in-out.
    static float
    inOutCubic(float t) noexcept;

    /// @brief Sinusoidal ease-in.
    static float
    inSine(float t) noexcept;
    /// @brief Sinusoidal ease-out.
    static float
    outSine(float t) noexcept;
    /// @brief Sinusoidal ease-in-out.
    static float
    inOutSine(float t) noexcept;

    /// @brief Exponential ease-in.
    static float
    inExpo(float t) noexcept;
    /// @brief Exponential ease-out.
    static float
    outExpo(float t) noexcept;
    /// @brief Exponential ease-in-out.
    static float
    inOutExpo(float t) noexcept;

    /// @brief Back ease-in (overshoots behind before moving forward).
    static float
    inBack(float t) noexcept;
    /// @brief Back ease-out (overshoots past destination then settles).
    static float
    outBack(float t) noexcept;
    /// @brief Back ease-in-out.
    static float
    inOutBack(float t) noexcept;

    /// @brief Elastic ease-in (spring-like oscillation at start).
    static float
    inElastic(float t) noexcept;
    /// @brief Elastic ease-out (spring-like oscillation at end).
    static float
    outElastic(float t) noexcept;
    /// @brief Elastic ease-in-out.
    static float
    inOutElastic(float t) noexcept;

    /// @brief Bounce ease-out (bouncing ball decay at end).
    static float
    outBounce(float t) noexcept;
    /// @brief Bounce ease-in (bouncing ball at start).
    static float
    inBounce(float t) noexcept;
    /// @brief Bounce ease-in-out.
    static float
    inOutBounce(float t) noexcept;
};

}  // namespace lili
