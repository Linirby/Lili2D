#pragma once

#include <cstdint>
#include <string>
#include <string_view>

#include "lili2d/core/asset_registry.hpp"
#include "lili2d/render/scene/common/material.hpp"

namespace lili {

/// @brief Registry for managing materials globally.
class MaterialRegistry : public AssetRegistry<Material, uint16_t> {
public:
    /// @brief Deleted copy constructor.
    MaterialRegistry(const MaterialRegistry&) = delete;
    /// @brief Deleted copy assignment operator.
    MaterialRegistry&
    operator=(const MaterialRegistry&) = delete;

    /// @brief Gets the singleton instance.
    /// @return Reference to the MaterialRegistry.
    static MaterialRegistry&
    get();

    /// @brief Registers a new material.
    /// @param key The unique string key.
    /// @param material The material to register.
    /// @return The assigned material ID.
    inline uint16_t
    registerMaterial(const std::string& key, const Material& material) {
        return registerAsset(key, material);
    }

    /// @brief Checks if a material exists.
    /// @param key The material key.
    /// @return True if the material exists.
    [[nodiscard]] inline bool
    hasMaterial(std::string_view key) const noexcept {
        return hasAsset(key);
    }
    /// @brief Gets a material ID by key.
    /// @param key The material key.
    /// @return The material ID.
    [[nodiscard]] inline uint16_t
    getMaterialID(std::string_view key) const {
        return getAssetID(key);
    }
    /// @brief Gets a material by key.
    /// @param key The material key.
    /// @return Reference to the material.
    [[nodiscard]] inline const Material&
    getMaterial(std::string_view key) const {
        return getAsset(key);
    }
    /// @brief Gets a material by 16-bit ID.
    /// @param material_id The material ID.
    /// @return Reference to the material.
    [[nodiscard]] inline const Material&
    getMaterial(uint16_t material_id) const {
        return getAsset(material_id);
    }
    /// @brief Gets a material by 8-bit ID.
    /// @param material_id The material ID.
    /// @return Reference to the material.
    [[nodiscard]] inline const Material&
    getMaterial(uint8_t material_id) const {
        return getAsset(static_cast<uint16_t>(material_id));
    }

    /// @brief Gets the total number of registered materials.
    /// @return The number of materials.
    [[nodiscard]] inline size_t
    materialCount() const noexcept {
        return assetCount();
    }
    /// @brief Gets the raw material data array.
    /// @return Pointer to the material data.
    [[nodiscard]] inline const Material*
    materialData() const noexcept {
        return assetData();
    }

private:
    MaterialRegistry();
};

}  // namespace lili
