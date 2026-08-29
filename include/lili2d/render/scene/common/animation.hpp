#pragma once

#include <vector>

#include "lili2d/render/scene/common/atlas_map.hpp"

namespace lili {

/// @brief Represents an animation as a sequence of frames.
///
/// An Animation is a simple container for a list of AnimationFrame objects.
/// Use AtlasMap to load and slice a spritesheet, then extract Animation
/// objects from it.
class Animation {
public:
    /// @brief Default constructor (empty animation).
    Animation() noexcept = default;
    /// @brief Constructs an animation from a pre-built list of frames.
    /// @param frames The sequence of frames.
    explicit Animation(const std::vector<SliceUV>& frames);

    /// @brief Copy constructor.
    Animation(const Animation&) = default;
    /// @brief Copy assignment operator.
    Animation&
    operator=(const Animation&) = default;
    /// @brief Move constructor.
    Animation(Animation&&) noexcept = default;
    /// @brief Move assignment operator.
    Animation&
    operator=(Animation&&) noexcept = default;

    /// @brief Gets the number of frames.
    /// @return The frame count.
    [[nodiscard]] inline size_t
    frameCount() const noexcept {
        return frames.size();
    }

    /// @brief Gets a frame by index.
    /// @param index The frame index.
    /// @return Reference to the SliceUV.
    [[nodiscard]] inline const SliceUV&
    getFrame(size_t index) const noexcept {
        return frames[index];
    }

private:
    std::vector<SliceUV> frames;
};

}  // namespace lili
