#include "lili2d/core/asset_registry.hpp"
#include "lili2d/render/scene/common/animation.hpp"

namespace lili {

/// @brief Registry for managing animations globally.
class AnimationRegistry : public AssetRegistry<Animation, uint16_t> {
public:
	AnimationRegistry(const AnimationRegistry&) = delete;
	AnimationRegistry& operator=(const AnimationRegistry&) = delete;

	/// @brief Gets the singleton instance.
	/// @return Reference to the AnimationRegistry.
	static AnimationRegistry &get();

	/// @brief Registers a new animation.
	/// @param key The unique string key.
	/// @param animation The animation to register.
	/// @return The assigned animation ID.
	uint16_t registerAnimation(
		const std::string &key, const Animation &animation
	);

	/// @brief Checks if an animation exists.
	/// @param key The animation key.
	/// @return True if the animation exists.
	bool hasAnimation(const std::string &key) const;
	/// @brief Gets an animation ID by key.
	/// @param key The animation key.
	/// @return The animation ID.
	uint16_t getAnimationId(const std::string &key) const;
	/// @brief Gets an animation by key.
	/// @param key The animation key.
	/// @return Reference to the animation.
	const Animation &getAnimation(const std::string &key) const;
	/// @brief Gets an animation by 16-bit ID.
	/// @param id The animation ID.
	/// @return Reference to the animation.
	const Animation &getAnimation(uint16_t id) const;
	/// @brief Gets an animation by 8-bit ID.
	/// @param id The animation ID.
	/// @return Reference to the animation.
	const Animation &getAnimation(uint8_t id) const;

	/// @brief Gets the total number of registered animations.
	/// @return The number of animations.
	size_t animationCount() const;
	/// @brief Gets the raw animation data array.
	/// @return Pointer to the animation data.
	const Animation *animationData() const;

private:
	/// @brief Private constructor to enforce singleton pattern.
	AnimationRegistry();
};

}  // namespace lili
