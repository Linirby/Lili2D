#pragma once

#include <filesystem>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace lili {

/// @brief Type-erased base interface for resource managers.
class IResourceManager {
public:
    virtual ~IResourceManager() = default;

    /// @brief Unloads all resources belonging to a scope.
    /// @param scope Scope tag.
    /// @return Number of resources unloaded.
    virtual size_t
    unloadScope(const std::string& scope) = 0;

    /// @brief Clears all managed resources.
    virtual void
    clear() = 0;

    /// @brief Polls file modification times for watched assets.
    virtual void
    checkHotReload() = 0;

    /// @brief Enables or disables hot reloading file polling.
    /// @param enabled True to enable.
    virtual void
    setHotReloadEnabled(bool enabled) = 0;

    /// @brief Checks if hot reloading is enabled.
    /// @return True if enabled.
    virtual bool
    isHotReloadEnabled() const = 0;

    /// @brief Gets total number of managed resources.
    /// @return Resource count.
    virtual size_t
    count() const = 0;
};

/// @brief Generic scoped resource manager with hot-reloading support.
/// @tparam T Type of resource managed.
template <typename T>
class ResourceManager : public IResourceManager {
public:
    /// @brief Function type for loading a resource from a file path.
    using LoaderFunc = std::function<std::unique_ptr<T>(const std::string&)>;
    /// @brief Function type for reloading a resource in-place from a file path.
    using ReloaderFunc = std::function<bool(T&, const std::string&)>;

    /// @brief Record tracking a managed resource and its metadata.
    struct ResourceRecord {
        /// @brief Unique pointer to the managed resource instance.
        std::unique_ptr<T> resource;
        /// @brief Path to the resource file on disk.
        std::string filepath;
        /// @brief Scope tag assigned to the resource.
        std::string scope;
        /// @brief Last write timestamp of the file for hot reloading.
        std::filesystem::file_time_type last_write_time{};
        /// @brief Loader function stored for hot reloading.
        LoaderFunc loader;
        /// @brief Reloader function stored for hot reloading.
        ReloaderFunc reloader;
    };

    /// @brief Default constructor.
    ResourceManager() = default;
    /// @brief Virtual destructor.
    ~ResourceManager() override = default;

    /// @brief Deleted copy constructor.
    ResourceManager(const ResourceManager&) = delete;
    /// @brief Deleted copy assignment operator.
    ResourceManager&
    operator=(const ResourceManager&) = delete;
    /// @brief Move constructor.
    ResourceManager(ResourceManager&&) noexcept = default;
    /// @brief Move assignment operator.
    ResourceManager&
    operator=(ResourceManager&&) noexcept = default;

    /// @brief Loads or retrieves a cached resource by key.
    /// @param key Unique string identifier.
    /// @param filepath Path to the resource file.
    /// @param loader Function that constructs a unique_ptr<T> from a filepath.
    /// @param scope Resource scope tag (default: "global").
    /// @param reloader Optional custom in-place reloader for hot reloading.
    /// @return Raw pointer to the loaded resource.
    T*
    load(
        const std::string& key, const std::string& filepath, LoaderFunc loader,
        const std::string& scope = "global", ReloaderFunc reloader = nullptr
    );

    /// @brief Loads or retrieves a cached resource using filepath as key.
    /// @param filepath Path to the resource file.
    /// @param loader Function that constructs a unique_ptr<T> from a filepath.
    /// @param scope Resource scope tag (default: "global").
    /// @param reloader Optional custom in-place reloader for hot reloading.
    /// @return Raw pointer to the loaded resource.
    T*
    load(
        const std::string& filepath, LoaderFunc loader,
        const std::string& scope = "global", ReloaderFunc reloader = nullptr
    );

    /// @brief Emplaces a pre-constructed resource into the cache.
    /// @param key Unique string identifier.
    /// @param resource Ownership of pre-constructed unique_ptr<T>.
    /// @param scope Resource scope tag (default: "global").
    /// @return Raw pointer to the emplaced resource.
    T*
    emplace(
        const std::string& key, std::unique_ptr<T> resource,
        const std::string& scope = "global"
    );

    /// @brief Retrieves a cached resource by key.
    /// @param key Resource identifier.
    /// @return Raw pointer to resource, or nullptr if not found.
    T*
    get(const std::string& key) const;

    /// @brief Retrieves a reference to a cached resource by key.
    /// @param key Resource identifier.
    /// @return Reference to the resource. Throws std::runtime_error if not
    /// found.
    T&
    getRef(const std::string& key) const;

    /// @brief Checks if a resource with the key exists.
    /// @param key Resource identifier.
    /// @return True if resource exists in cache.
    bool
    has(const std::string& key) const;

    /// @brief Unloads a single resource by key.
    /// @param key Resource identifier.
    /// @return True if resource was found and unloaded.
    bool
    unload(const std::string& key);

    /// @brief Unloads all resources matching a specific scope.
    /// @param scope Scope tag to clear.
    /// @return Number of resources unloaded.
    size_t
    unloadScope(const std::string& scope) override;

    /// @brief Clears all cached resources.
    void
    clear() override;

    /// @brief Gets total number of managed resources.
    /// @return Resource count.
    size_t
    count() const override;

    /// @brief Polls file modification times for watched assets and reloads
    /// modified files.
    void
    checkHotReload() override;

    /// @brief Enables or disables hot reloading file polling.
    /// @param enabled True to enable file watcher checks.
    void
    setHotReloadEnabled(bool enabled) override;

    /// @brief Checks if hot reloading is enabled.
    /// @return True if enabled.
    bool
    isHotReloadEnabled() const override;

private:
    /// @brief Hash map of cached resources by string key.
    std::unordered_map<std::string, ResourceRecord> resources;
    /// @brief Flag indicating if hot reloading file watcher is enabled.
    bool hot_reload_enabled = false;
};

// --- Template Implementation ---

template <typename T>
T*
ResourceManager<T>::load(
    const std::string& key, const std::string& filepath, LoaderFunc loader,
    const std::string& scope, ReloaderFunc reloader
) {
    auto it = resources.find(key);
    if (it != resources.end()) return it->second.resource.get();

    if (!loader)
        throw std::runtime_error(
            "ResourceManager::load failed: loader function is null for key: " +
            key
        );

    std::unique_ptr<T> res = loader(filepath);
    if (!res)
        throw std::runtime_error(
            "ResourceManager::load failed to load asset at path: " + filepath
        );

    ResourceRecord record;
    record.resource = std::move(res);
    record.filepath = filepath;
    record.scope = scope;
    record.loader = std::move(loader);
    record.reloader = std::move(reloader);

    if (!filepath.empty()) {
        std::error_code ec;
        auto write_time = std::filesystem::last_write_time(filepath, ec);
        if (!ec) record.last_write_time = write_time;
    }

    T* ptr = record.resource.get();
    resources[key] = std::move(record);
    return ptr;
}

template <typename T>
T*
ResourceManager<T>::load(
    const std::string& filepath, LoaderFunc loader, const std::string& scope,
    ReloaderFunc reloader
) {
    return load(
        filepath, filepath, std::move(loader), scope, std::move(reloader)
    );
}

template <typename T>
T*
ResourceManager<T>::emplace(
    const std::string& key, std::unique_ptr<T> resource,
    const std::string& scope
) {
    if (!resource)
        throw std::runtime_error(
            "ResourceManager::emplace failed: resource is null for key: " + key
        );

    ResourceRecord record;
    record.resource = std::move(resource);
    record.scope = scope;

    T* ptr = record.resource.get();
    resources[key] = std::move(record);
    return ptr;
}

template <typename T>
T*
ResourceManager<T>::get(const std::string& key) const {
    auto it = resources.find(key);
    if (it != resources.end()) return it->second.resource.get();
    return nullptr;
}

template <typename T>
T&
ResourceManager<T>::getRef(const std::string& key) const {
    T* ptr = get(key);
    if (!ptr)
        throw std::runtime_error(
            "ResourceManager::getRef asset not found: " + key
        );
    return *ptr;
}

template <typename T>
bool
ResourceManager<T>::has(const std::string& key) const {
    return resources.contains(key);
}

template <typename T>
bool
ResourceManager<T>::unload(const std::string& key) {
    auto it = resources.find(key);
    if (it != resources.end()) {
        resources.erase(it);
        return true;
    }
    return false;
}

template <typename T>
size_t
ResourceManager<T>::unloadScope(const std::string& scope) {
    size_t unloaded = 0;
    for (auto it = resources.begin(); it != resources.end();) {
        if (it->second.scope == scope) {
            it = resources.erase(it);
            ++unloaded;
        } else
            ++it;
    }
    return unloaded;
}

template <typename T>
void
ResourceManager<T>::clear() {
    resources.clear();
}

template <typename T>
size_t
ResourceManager<T>::count() const {
    return resources.size();
}

template <typename T>
void
ResourceManager<T>::setHotReloadEnabled(bool enabled) {
    hot_reload_enabled = enabled;
}

template <typename T>
bool
ResourceManager<T>::isHotReloadEnabled() const {
    return hot_reload_enabled;
}

template <typename T>
void
ResourceManager<T>::checkHotReload() {
    if (!hot_reload_enabled) return;

    for (auto& [key, record] : resources) {
        if (record.filepath.empty()) continue;

        std::error_code ec;
        auto current_write_time =
            std::filesystem::last_write_time(record.filepath, ec);
        if (ec) continue;

        if (current_write_time > record.last_write_time) {
            record.last_write_time = current_write_time;

            if (record.reloader && record.resource)
                record.reloader(*record.resource, record.filepath);
            else if (record.loader && record.resource) {
                std::unique_ptr<T> fresh = record.loader(record.filepath);
                if (fresh) {
                    if constexpr (std::is_move_assignable_v<T>)
                        *record.resource = std::move(*fresh);
                    else
                        record.resource = std::move(fresh);
                }
            }
        }
    }
}

}  // namespace lili
