#pragma once

#include <filesystem>
#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "lili2d/core/string_hash.hpp"

namespace lili {

/// @brief Type-erased base interface for resource managers.
class IResourceManager {
public:
    virtual ~IResourceManager() = default;

    /// @brief Unloads all resources belonging to a scope.
    /// @param scope Scope tag.
    /// @return Number of resources unloaded.
    virtual size_t
    unloadScope(std::string_view scope) = 0;

    /// @brief Clears all managed resources.
    virtual void
    clear() noexcept = 0;

    /// @brief Polls file modification times for watched assets.
    virtual void
    checkHotReload() = 0;

    /// @brief Enables or disables hot reloading file polling.
    /// @param enabled True to enable.
    virtual void
    setHotReloadEnabled(bool enabled) noexcept = 0;

    /// @brief Checks if hot reloading is enabled.
    /// @return True if enabled.
    [[nodiscard]] virtual bool
    isHotReloadEnabled() const noexcept = 0;

    /// @brief Gets total number of managed resources.
    /// @return Resource count.
    [[nodiscard]] virtual size_t
    count() const noexcept = 0;
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

    /// @brief Record tracking a watched file and its last write time.
    struct WatchedFile {
        /// @brief Path to the watched file on disk.
        std::string path;
        /// @brief Last write timestamp of the file.
        std::filesystem::file_time_type last_write_time{};

        WatchedFile() = default;
        WatchedFile(
            std::string path, std::filesystem::file_time_type last_write_time
        )
            : path(std::move(path)), last_write_time(last_write_time) {}
        WatchedFile(const WatchedFile&) = default;
        WatchedFile&
        operator=(const WatchedFile&) = default;
        WatchedFile(WatchedFile&&) noexcept = default;
        WatchedFile&
        operator=(WatchedFile&&) noexcept = default;
    };

    /// @brief Record tracking a managed resource and its metadata.
    struct ResourceRecord {
        /// @brief Unique pointer to the managed resource instance.
        std::unique_ptr<T> resource;
        /// @brief Path to the primary resource file on disk.
        std::string filepath;
        /// @brief List of watched files and their timestamps for hot reloading.
        std::vector<WatchedFile> watched_files;
        /// @brief Scope tag assigned to the resource.
        std::string scope;
        /// @brief Last write timestamp of the primary file for hot reloading.
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

    /// @brief Loads or retrieves a cached resource watching multiple filepaths.
    /// @param key Unique string identifier.
    /// @param filepaths Vector of filepaths to watch for hot reloading.
    /// @param loader Function that constructs a unique_ptr<T> from the primary
    /// filepath.
    /// @param scope Resource scope tag (default: "global").
    /// @param reloader Optional custom in-place reloader for hot reloading.
    /// @return Raw pointer to the loaded resource.
    T*
    load(
        const std::string& key, const std::vector<std::string>& filepaths,
        LoaderFunc loader, const std::string& scope = "global",
        ReloaderFunc reloader = nullptr
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
    [[nodiscard]] T*
    get(std::string_view key) const noexcept;

    /// @brief Retrieves a reference to a cached resource by key.
    /// @param key Resource identifier.
    /// @return Reference to the resource. Throws std::runtime_error if not
    /// found.
    [[nodiscard]] T&
    getRef(std::string_view key) const;

    /// @brief Checks if a resource with the key exists.
    /// @param key Resource identifier.
    /// @return True if resource exists in cache.
    [[nodiscard]] bool
    has(std::string_view key) const noexcept;

    /// @brief Unloads a single resource by key.
    /// @param key Resource identifier.
    /// @return True if resource was found and unloaded.
    bool
    unload(std::string_view key);

    /// @brief Unloads all resources matching a specific scope.
    /// @param scope Scope tag to clear.
    /// @return Number of resources unloaded.
    size_t
    unloadScope(std::string_view scope) override;

    /// @brief Clears all cached resources.
    void
    clear() noexcept override;

    /// @brief Gets total number of managed resources.
    /// @return Resource count.
    [[nodiscard]] size_t
    count() const noexcept override;

    /// @brief Polls file modification times for watched assets and reloads
    /// modified files.
    void
    checkHotReload() override;

    /// @brief Enables or disables hot reloading file polling.
    /// @param enabled True to enable file watcher checks.
    void
    setHotReloadEnabled(bool enabled) noexcept override;

    /// @brief Checks if hot reloading is enabled.
    /// @return True if enabled.
    [[nodiscard]] bool
    isHotReloadEnabled() const noexcept override;

private:
    /// @brief Hash map of cached resources by string key.
    StringMap<ResourceRecord> resources;
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
        if (!ec) {
            record.last_write_time = write_time;
            record.watched_files.push_back({filepath, write_time});
        }
    }

    T* ptr = record.resource.get();
    resources[key] = std::move(record);
    return ptr;
}

template <typename T>
T*
ResourceManager<T>::load(
    const std::string& key, const std::vector<std::string>& filepaths,
    LoaderFunc loader, const std::string& scope, ReloaderFunc reloader
) {
    auto it = resources.find(key);
    if (it != resources.end()) return it->second.resource.get();

    if (!loader)
        throw std::runtime_error(
            "ResourceManager::load failed: loader function is null for key: " +
            key
        );

    std::string primary_path = filepaths.empty() ? "" : filepaths[0];
    std::unique_ptr<T> res = loader(primary_path);
    if (!res)
        throw std::runtime_error(
            "ResourceManager::load failed to load asset at path: " +
            primary_path
        );

    ResourceRecord record;
    record.resource = std::move(res);
    record.filepath = primary_path;
    record.scope = scope;
    record.loader = std::move(loader);
    record.reloader = std::move(reloader);

    for (const auto& fp : filepaths) {
        if (fp.empty()) continue;
        std::error_code ec;
        auto write_time = std::filesystem::last_write_time(fp, ec);
        if (!ec) {
            record.watched_files.push_back({fp, write_time});
            if (record.last_write_time == std::filesystem::file_time_type{}) {
                record.last_write_time = write_time;
            }
        }
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
ResourceManager<T>::get(std::string_view key) const noexcept {
    auto it = resources.find(key);
    if (it == resources.end()) return nullptr;
    return it->second.resource.get();
}

template <typename T>
T&
ResourceManager<T>::getRef(std::string_view key) const {
    T* ptr = get(key);
    if (ptr) return *ptr;
    throw std::runtime_error(
        std::format("ResourceManager::getRef asset not found: {}", key)
    );
}

template <typename T>
bool
ResourceManager<T>::has(std::string_view key) const noexcept {
    return resources.find(key) != resources.end();
}

template <typename T>
bool
ResourceManager<T>::unload(std::string_view key) {
    auto it = resources.find(key);
    if (it == resources.end()) return false;
    resources.erase(it);
    return true;
}

template <typename T>
size_t
ResourceManager<T>::unloadScope(std::string_view scope) {
    size_t unloaded = 0;
    for (auto it = resources.begin(); it != resources.end();) {
        if (it->second.scope != scope) {
            ++it;
        } else {
            it = resources.erase(it);
            ++unloaded;
        }
    }
    return unloaded;
}

template <typename T>
void
ResourceManager<T>::clear() noexcept {
    resources.clear();
}

template <typename T>
size_t
ResourceManager<T>::count() const noexcept {
    return resources.size();
}

template <typename T>
void
ResourceManager<T>::setHotReloadEnabled(bool enabled) noexcept {
    hot_reload_enabled = enabled;
}

template <typename T>
bool
ResourceManager<T>::isHotReloadEnabled() const noexcept {
    return hot_reload_enabled;
}

template <typename T>
void
ResourceManager<T>::checkHotReload() {
    if (!hot_reload_enabled) return;

    for (auto& [key, record] : resources) {
        if (record.watched_files.empty() && record.filepath.empty()) continue;

        bool needs_reload = false;
        std::vector<std::filesystem::file_time_type> current_times;

        if (!record.watched_files.empty()) {
            current_times.reserve(record.watched_files.size());
            for (const auto& watched : record.watched_files) {
                if (watched.path.empty()) {
                    current_times.push_back({});
                    continue;
                }
                std::error_code ec;
                auto current_write_time =
                    std::filesystem::last_write_time(watched.path, ec);
                if (ec) {
                    current_times.push_back(watched.last_write_time);
                    continue;
                }
                current_times.push_back(current_write_time);
                if (current_write_time > watched.last_write_time) {
                    needs_reload = true;
                }
            }
        } else if (!record.filepath.empty()) {
            std::error_code ec;
            auto current_write_time =
                std::filesystem::last_write_time(record.filepath, ec);
            if (!ec) {
                current_times.push_back(current_write_time);
                if (current_write_time > record.last_write_time) {
                    needs_reload = true;
                }
            }
        }

        if (!needs_reload) continue;

        try {
            bool success = false;
            std::string primary_path = !record.watched_files.empty()
                                           ? record.watched_files[0].path
                                           : record.filepath;
            if (record.reloader && record.resource) {
                success = record.reloader(*record.resource, primary_path);
            } else if (record.loader && record.resource) {
                std::unique_ptr<T> fresh = record.loader(primary_path);
                if (fresh) {
                    if constexpr (std::is_move_assignable_v<T>)
                        *record.resource = std::move(*fresh);
                    else
                        record.resource = std::move(fresh);
                    success = true;
                }
            }
            if (success) {
                if (!record.watched_files.empty()) {
                    for (size_t i = 0; i < record.watched_files.size(); ++i) {
                        record.watched_files[i].last_write_time =
                            current_times[i];
                    }
                    record.last_write_time =
                        record.watched_files[0].last_write_time;
                } else if (!current_times.empty()) {
                    record.last_write_time = current_times[0];
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Hot reload failed for key '" << key
                      << "': " << e.what() << std::endl;
        }
    }
}

}  // namespace lili
