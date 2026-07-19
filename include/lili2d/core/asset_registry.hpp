#pragma once

#include <cstdint>
#include <unordered_map>
#include <string>
#include <vector>
#include <stdexcept>
#include <limits>

namespace lili {

/// @brief Generic registry for loading, storing, and referencing game assets.
/// @tparam T The type of the asset.
/// @tparam IdType The type of the ID used to reference the asset.
template<typename T, typename IdType = uint32_t>
class AssetRegistry {
protected:
	/// @brief Maps asset name/key to registered internal ID.
	std::unordered_map<std::string, IdType> key_to_id;
	/// @brief Contiguous array storing assets indexed by ID.
	std::vector<T> id_to_asset;

public:
	AssetRegistry() = default;
	virtual ~AssetRegistry() = default;
	AssetRegistry(const AssetRegistry &) = delete;
	AssetRegistry &operator=(const AssetRegistry &) = delete;
	AssetRegistry(AssetRegistry &&) = delete;
	AssetRegistry &operator=(AssetRegistry &&) = delete;

	/// @brief Registers an asset using an rvalue reference.
	/// @param key The key to associate with the asset.
	/// @param asset The asset to register.
	/// @return The registered asset's ID.
	virtual IdType registerAsset(const std::string &key, T &&asset);

	/// @brief Registers an asset using a const reference.
	/// @param key The key to associate with the asset.
	/// @param asset The asset to register.
	/// @return The registered asset's ID.
	virtual IdType registerAsset(const std::string &key, const T &asset);

	/// @brief Checks if an asset with the given key exists.
	/// @param key The key to check.
	/// @return True if the asset exists, false otherwise.
	bool hasAsset(const std::string &key) const;

	/// @brief Gets the ID of the asset with the given key.
	/// @param key The key of the asset.
	/// @return The asset's ID.
	IdType getAssetID(const std::string &key) const;

	/// @brief Gets a const reference to the asset with the given key.
	/// @param key The key of the asset.
	/// @return Const reference to the asset.
	const T &getAsset(const std::string &key) const;

	/// @brief Gets a const reference to the asset with the given ID.
	/// @param key The ID of the asset.
	/// @return Const reference to the asset.
	const T &getAsset(IdType key) const;

	/// @brief Gets a mutable reference to the asset with the given ID.
	/// @param key The ID of the asset.
	/// @return Mutable reference to the asset.
	T &getAsset(IdType key);

	/// @brief Gets the total number of registered assets.
	/// @return The asset count.
	size_t assetCount() const;

	/// @brief Gets a pointer to the raw array of registered assets.
	/// @return Pointer to the asset data.
	const T *assetData() const;

	/// @brief Clears all registered assets from the registry.
	void clear();
};

template<typename T, typename IdType>
IdType AssetRegistry<T, IdType>::registerAsset(
	const std::string &key, T &&asset
) {
	auto it = key_to_id.find(key);
	if (it != key_to_id.end()) {
		id_to_asset[it->second] = std::move(asset);
		return it->second;
	}

	if (id_to_asset.size() >= std::numeric_limits<IdType>::max())
		throw std::runtime_error("AssetRegistry has reached maximum capacity.");

	IdType new_id = static_cast<IdType>(id_to_asset.size());
	id_to_asset.push_back(std::move(asset));
	key_to_id[key] = new_id;
	return new_id;
}

template<typename T, typename IdType>
IdType AssetRegistry<T, IdType>::registerAsset(
	const std::string &key, const T &asset
) {
	auto it = key_to_id.find(key);
	if (it != key_to_id.end()) {
		id_to_asset[it->second] = asset;
		return it->second;
	}

	if (id_to_asset.size() >= std::numeric_limits<IdType>::max())
		throw std::runtime_error("AssetRegistry has reached maximum capacity.");

	IdType new_id = static_cast<IdType>(id_to_asset.size());
	id_to_asset.push_back(asset);
	key_to_id[key] = new_id;
	return new_id;
}

template<typename T, typename IdType>
bool AssetRegistry<T, IdType>::hasAsset(const std::string &key) const {
	return key_to_id.contains(key);
}

template<typename T, typename IdType>
IdType AssetRegistry<T, IdType>::getAssetID(const std::string &key) const {
	auto it = key_to_id.find(key);
	if (it == key_to_id.end())
		throw std::runtime_error("Asset key not found: " + key);
	return it->second;
}

template<typename T, typename IdType>
const T &AssetRegistry<T, IdType>::getAsset(const std::string &key) const {
	return this->getAsset(getAssetID(key));
}

template<typename T, typename IdType>
const T &AssetRegistry<T, IdType>::getAsset(IdType id) const {
	if (id >= id_to_asset.size())
		throw std::runtime_error("Asset ID out of range");
	return id_to_asset[id];
}

template<typename T, typename IdType>
T &AssetRegistry<T, IdType>::getAsset(IdType id) {
	if (id >= id_to_asset.size())
		throw std::runtime_error("Asset ID out of range");
	return id_to_asset[id];
}

template<typename T, typename IdType>
size_t AssetRegistry<T, IdType>::assetCount() const {
	return id_to_asset.size();
}

template<typename T, typename IdType>
const T *AssetRegistry<T, IdType>::assetData() const {
	return id_to_asset.data();
}

template<typename T, typename IdType>
void AssetRegistry<T, IdType>::clear() {
	id_to_asset.clear();
	key_to_id.clear();
}

}  // namespace lili
