#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace lili {

/**
 * \brief Properties for a material.
 */
struct AnimationProps {
	int frames = 1;
	int start_x_atlas, start_y_atlas = 0;
	int end_x_atlas, end_y_atlas = 0;
};

/**
 * \brief Defines how a model is rendered.
 */
struct Animation {
	AnimationProps properties;      ///< Animation properties.
	int curent_frame = 0;

	/// \brief Default constructor.
	Animation() = default;
};

/**
 * \brief Registry for managing materials globally.
 */
class AnimationRegistry {
public:
	AnimationRegistry(const AnimationRegistry&) = delete;
	AnimationRegistry& operator=(const AnimationRegistry&) = delete;

	/**
	 * \brief Gets the singleton instance.
	 * \return Reference to the AnimationRegistry.
	 */
	static AnimationRegistry &get();

	/**
	 * \brief Registers a new material.
	 * \param key The unique string key.
	 * \param material The material to register.
	 * \return The assigned material ID.
	 */
	uint16_t register_animation(const std::string &key, const Animation &animation);

	/**
	 * \brief Checks if a material exists.
	 * \param key The material key.
	 * \return True if the material exists.
	 */
	bool has_animation(const std::string &key) const;
	/**
	 * \brief Gets a material ID by key.
	 * \param key The material key.
	 * \return The material ID.
	 */
	uint16_t get_animation_id(const std::string &key) const;
	/**
	 * \brief Gets a material by key.
	 * \param key The material key.
	 * \return Reference to the material.
	 */
	const Animation &get_animation(const std::string &key) const;
	/**
	 * \brief Gets a material by 16-bit ID.
	 * \param material_id The material ID.
	 * \return Reference to the material.
	 */
	const Animation &get_animation(uint16_t id) const;
	/**
	 * \brief Gets a material by 8-bit ID.
	 * \param material_id The material ID.
	 * \return Reference to the material.
	 */
	const Animation &get_animation(uint8_t id) const;

	/**
	 * \brief Gets the total number of registered materials.
	 * \return The number of materials.
	 */
	size_t animation_count() const;
	/**
	 * \brief Gets the raw material data array.
	 * \return Pointer to the material data.
	 */
	const Animation *animation_data() const;

private:
	std::unordered_map<std::string, uint16_t> key_to_id;
	std::vector<Animation> id_to_animation;

	AnimationRegistry();
};

}  // namespace lili
