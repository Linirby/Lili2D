#include "lili2d/render/scene/common/animation_registry.hpp"

namespace lili {

AnimationRegistry &AnimationRegistry::get() {
	static AnimationRegistry instance;
	return instance;
}

uint16_t AnimationRegistry::registerAnimation(
	const std::string &key, const Animation &animation
) {
	return registerAsset(key, animation);
}

bool AnimationRegistry::hasAnimation(const std::string &key) const {
	return hasAsset(key);
}

uint16_t AnimationRegistry::getAnimationId(const std::string &key) const {
	return getAssetID(key);
}

const Animation &AnimationRegistry::getAnimation(
	const std::string &key
) const {
	return getAsset(key);
}

const Animation &AnimationRegistry::getAnimation(uint16_t id) const {
	return getAsset(id);
}

const Animation &AnimationRegistry::getAnimation(uint8_t id) const {
	return getAsset(id);
}

size_t AnimationRegistry::animationCount() const {
	return assetCount();
}

const Animation *AnimationRegistry::animationData() const {
	return assetData();
}

AnimationRegistry::AnimationRegistry() {
	registerAnimation("core:none", Animation());
}

}  // namespace lili
