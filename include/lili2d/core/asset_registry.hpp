#pragma once

#include <cstdint>
#include <format>
#include <limits>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "lili2d/core/string_hash.hpp"

namespace lili {

/// @brief Generic registry for loading, storing, and referencing game assets.
/// @tparam T The type of the asset.
/// @tparam IdType The type of the ID used to reference the asset.
template<typename T, typename IdType = uint32_t>
class AssetRegistry
{
protected:
    /// @brief Map associating string keys to asset IDs.
    StringMap<IdType> ids;
    /// @brief Vector storing assets indexed by ID.
    std::vector<T> id_to_asset;

public:
    /// @brief Default constructor.
    AssetRegistry() = default;
    /// @brief Virtual destructor.
    virtual ~AssetRegistry() = default;
    /// @brief Deleted copy constructor.
    AssetRegistry(const AssetRegistry&) = delete;
    /// @brief Deleted copy assignment operator.
    AssetRegistry&
    operator=(const AssetRegistry&) = delete;
    /// @brief Deleted move constructor.
    AssetRegistry(AssetRegistry&&) = delete;
    /// @brief Deleted move assignment operator.
    AssetRegistry&
    operator=(AssetRegistry&&) = delete;

    /// @brief Registers an asset using an rvalue reference.
    /// @param key The key to associate with the asset.
    /// @param asset The asset to register.
    /// @return The registered asset's ID.
    virtual IdType
    registerAsset(const std::string& key, T&& asset)
    {
        auto it = ids.find(key);
        if (it != ids.end()) {
            id_to_asset[it->second] = std::move(asset);
            return it->second;
        }

        if (id_to_asset.size() >= std::numeric_limits<IdType>::max())
            throw std::runtime_error(
                "AssetRegistry has reached maximum capacity."
            );

        IdType new_id = static_cast<IdType>(id_to_asset.size());
        id_to_asset.push_back(std::move(asset));
        ids[key] = new_id;
        return new_id;
    }

    /// @brief Registers an asset using a const reference.
    /// @param key The key to associate with the asset.
    /// @param asset The asset to register.
    /// @return The registered asset's ID.
    virtual IdType
    registerAsset(const std::string& key, const T& asset)
    {
        auto it = ids.find(key);
        if (it != ids.end()) {
            id_to_asset[it->second] = asset;
            return it->second;
        }

        if (id_to_asset.size() >= std::numeric_limits<IdType>::max())
            throw std::runtime_error(
                "AssetRegistry has reached maximum capacity."
            );

        IdType new_id = static_cast<IdType>(id_to_asset.size());
        id_to_asset.push_back(asset);
        ids[key] = new_id;
        return new_id;
    }

    /// @brief Checks if an asset with the given key exists.
    /// @param key The key to check.
    /// @return True if the asset exists, false otherwise.
    [[nodiscard]] bool
    hasAsset(std::string_view key) const noexcept
    {
        return ids.find(key) != ids.end();
    }

    /// @brief Gets the ID of the asset with the given key.
    /// @param key The key of the asset.
    /// @return The asset's ID.
    [[nodiscard]] IdType
    getAssetID(std::string_view key) const
    {
        auto it = ids.find(key);
        if (it == ids.end())
            throw std::runtime_error(
                std::format("Asset key not found: {}", key)
            );
        return it->second;
    }

    /// @brief Gets a const reference to the asset with the given key.
    /// @param key The key of the asset.
    /// @return Const reference to the asset.
    [[nodiscard]] const T&
    getAsset(std::string_view key) const
    {
        return this->getAsset(getAssetID(key));
    }

    /// @brief Gets a mutable reference to the asset with the given ID.
    /// @param key The ID of the asset.
    /// @return Mutable reference to the asset.
    [[nodiscard]] T&
    getAsset(IdType key)
    {
        if (key >= id_to_asset.size())
            throw std::runtime_error("Asset ID out of range");
        return id_to_asset[key];
    }

    /// @brief Gets a const reference to the asset with the given ID.
    /// @param key The ID of the asset.
    /// @return Const reference to the asset.
    [[nodiscard]] const T&
    getAsset(IdType key) const
    {
        if (key >= id_to_asset.size())
            throw std::runtime_error("Asset ID out of range");
        return id_to_asset[key];
    }

    /// @brief Gets the total number of registered assets.
    /// @return The asset count.
    [[nodiscard]] size_t
    assetCount() const noexcept
    {
        return id_to_asset.size();
    }

    /// @brief Gets a pointer to the raw array of registered assets.
    /// @return Pointer to the asset data.
    [[nodiscard]] const T*
    assetData() const noexcept
    {
        return id_to_asset.data();
    }

    /// @brief Clears all registered assets from the registry.
    void
    clear() noexcept
    {
        id_to_asset.clear();
        ids.clear();
    }
};

} // namespace lili
