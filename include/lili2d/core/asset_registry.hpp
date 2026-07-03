#pragma once

#include <cstdint>
#include <unordered_map>
#include <string>
#include <vector>
#include <stdexcept>

namespace lili {

template<typename T, typename IdType = uint32_t>
class AssetRegistry {
protected:
	std::unordered_map<std::string, IdType> key_to_id;
	std::vector<T> id_to_asset;

public:
	virtual ~AssetRegistry() = default;

	virtual IdType registerAsset(const std::string &key, T &&asset);
	virtual IdType registerAsset(const std::string &key, const T &asset);
	bool hasAsset(const std::string &key) const;
	IdType getAssetID(const std::string &key) const;
	const T &getAsset(const std::string &key) const;
	const T &getAsset(IdType key) const;
	T &getAsset(IdType key);
	size_t assetCount() const;
	const T *assetData() const;
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
	return getAsset(getAssetID(key));
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
