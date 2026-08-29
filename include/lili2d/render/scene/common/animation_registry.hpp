#pragma once

#include "lili2d/core/asset_registry.hpp"
#include "lili2d/render/scene/common/animation.hpp"

namespace lili {

/// @brief Registry for managing animations globally.
class AnimationRegistry : public AssetRegistry<Animation, uint16_t> {
public:
    /// @brief Deleted copy constructor.
    AnimationRegistry(const AnimationRegistry&) = delete;
    /// @brief Deleted copy assignment operator.
    AnimationRegistry&
    operator=(const AnimationRegistry&) = delete;

    /// @brief Gets the singleton instance.
    /// @return Reference to the AnimationRegistry.
    [[nodiscard]] static AnimationRegistry&
    get();

    /// @brief Registers a new animation.
    /// @param key The unique string key.
    /// @param animation The animation to register.
    /// @return The assigned animation ID.
    inline uint16_t
    registerAnimation(const std::string& key, const Animation& animation) {
        return registerAsset(key, animation);
    }

    /// @brief Checks if an animation exists.
    /// @param key The animation key.
    /// @return True if the animation exists.
    [[nodiscard]] inline bool
    hasAnimation(std::string_view key) const noexcept {
        return hasAsset(key);
    }
    /// @brief Gets an animation ID by key.
    /// @param key The animation key.
    /// @return The animation ID.
    [[nodiscard]] inline uint16_t
    getAnimationId(std::string_view key) const noexcept {
        return getAssetID(key);
    }
    /// @brief Gets an animation by key.
    /// @param key The animation key.
    /// @return Reference to the animation.
    [[nodiscard]] inline const Animation&
    getAnimation(std::string_view key) const noexcept {
        return getAsset(key);
    }
    /// @brief Gets an animation by 16-bit ID.
    /// @param id The animation ID.
    /// @return Reference to the animation.
    [[nodiscard]] inline const Animation&
    getAnimation(uint16_t id) const noexcept {
        return getAsset(id);
    }
    /// @brief Gets an animation by 8-bit ID.
    /// @param id The animation ID.
    /// @return Reference to the animation.
    [[nodiscard]] inline const Animation&
    getAnimation(uint8_t id) const noexcept {
        return getAsset(id);
    }

    /// @brief Gets the total number of registered animations.
    /// @return The number of animations.
    [[nodiscard]] inline size_t
    animationCount() const noexcept {
        return assetCount();
    }
    /// @brief Gets the raw animation data array.
    /// @return Pointer to the animation data.
    [[nodiscard]] inline const Animation*
    animationData() const noexcept {
        return assetData();
    }

private:
    AnimationRegistry();
};

}  // namespace lili
