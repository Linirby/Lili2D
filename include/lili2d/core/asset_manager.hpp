#pragma once

#include <SDL3/SDL_gpu.h>

#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>

#include "lili2d/core/resource_manager.hpp"
#include "lili2d/render/core/shader.hpp"
#include "lili2d/render/core/texture.hpp"
#include "lili2d/render/scene/common/atlas_map.hpp"
#include "lili2d/render/scene/common/text.hpp"

namespace lili {

/// @brief Centralized facade for engine asset management.
class AssetManager {
public:
    /// @brief Gets the singleton instance.
    /// @return Reference to AssetManager.
    static AssetManager&
    get();

    /// @brief Deleted copy constructor.
    AssetManager(const AssetManager&) = delete;
    /// @brief Deleted copy assignment operator.
    AssetManager&
    operator=(const AssetManager&) = delete;
    /// @brief Deleted move constructor.
    AssetManager(AssetManager&&) = delete;
    /// @brief Deleted move assignment operator.
    AssetManager&
    operator=(AssetManager&&) = delete;

    /// @brief Gets the texture resource manager.
    /// @return Reference to ResourceManager<Texture>.
    ResourceManager<Texture>&
    textures();

    /// @brief Gets the shader resource manager.
    /// @return Reference to ResourceManager<Shader>.
    ResourceManager<Shader>&
    shaders();

    /// @brief Gets the bitmap font resource manager.
    /// @return Reference to ResourceManager<BitmapFont>.
    ResourceManager<BitmapFont>&
    fonts();

    /// @brief Gets the atlas map (sprite sheet) resource manager.
    /// @return Reference to ResourceManager<AtlasMap>.
    ResourceManager<AtlasMap>&
    atlases();

    /// @brief Gets or registers a generic ResourceManager for any custom type
    /// T.
    /// @tparam T The resource type.
    /// @return Reference to ResourceManager<T>.
    template <typename T>
    static ResourceManager<T>&
    getManager();

    // --- Static Convenience API ---

    /// @brief Loads or retrieves a texture by key.
    /// @param key Unique key.
    /// @param path Image file path.
    /// @param device SDL GPU device.
    /// @param scope Asset scope tag (default: "global").
    /// @return Raw pointer to Texture.
    static Texture*
    loadTexture(
        const std::string& key, const std::string& path, SDL_GPUDevice* device,
        const std::string& scope = "global"
    );

    /// @brief Loads or retrieves a texture using filepath as key.
    /// @param path Image file path.
    /// @param device SDL GPU device.
    /// @param scope Asset scope tag (default: "global").
    /// @return Raw pointer to Texture.
    static Texture*
    loadTexture(
        const std::string& path, SDL_GPUDevice* device,
        const std::string& scope = "global"
    );

    /// @brief Gets a cached texture by key.
    /// @param key Unique key.
    /// @return Raw pointer to Texture or nullptr.
    static Texture*
    getTexture(const std::string& key);

    /// @brief Loads or retrieves a shader by key.
    /// @param key Unique key.
    /// @param vertPath Vertex shader file path.
    /// @param fragPath Fragment shader file path.
    /// @param device SDL GPU device.
    /// @param vert_infos Binding info for the vertex shader.
    /// @param frag_infos Binding info for the fragment shader.
    /// @param scope Asset scope tag (default: "global").
    /// @return Raw pointer to Shader.
    static Shader*
    loadShader(
        const std::string& key, const std::string& vertPath,
        const std::string& fragPath, SDL_GPUDevice* device,
        ShaderInfo vert_infos = ShaderInfo({.num_uniform_buffers = 1}),
        ShaderInfo frag_infos = ShaderInfo({.num_samplers = 1}),
        const std::string& scope = "global"
    );

    /// @brief Gets a cached shader by key.
    /// @param key Unique key.
    /// @return Raw pointer to Shader or nullptr.
    static Shader*
    getShader(const std::string& key);

    /// @brief Loads or retrieves a bitmap font by key.
    /// @param key Unique key.
    /// @param renderer Pointer to Renderer.
    /// @param path Path to font image file.
    /// @param cols Number of columns in font atlas grid.
    /// @param rows Number of rows in font atlas grid.
    /// @param scope Asset scope tag (default: "global").
    /// @return Raw pointer to BitmapFont.
    static BitmapFont*
    loadFont(
        const std::string& key, Renderer* renderer, const std::string& path,
        uint8_t cols, uint8_t rows, const std::string& scope = "global"
    );

    /// @brief Gets a cached bitmap font by key.
    /// @param key Unique key.
    /// @return Raw pointer to BitmapFont or nullptr.
    static BitmapFont*
    getFont(const std::string& key);

    /// @brief Loads or retrieves an AtlasMap by key.
    /// @param key Unique key.
    /// @param renderer Pointer to Renderer.
    /// @param path Path to sprite sheet image file.
    /// @param cols Number of grid columns to slice.
    /// @param rows Number of grid rows to slice.
    /// @param scope Asset scope tag (default: "global").
    /// @return Raw pointer to AtlasMap.
    static AtlasMap*
    loadAtlas(
        const std::string& key, Renderer* renderer, const std::string& path,
        int cols, int rows, const std::string& scope = "global"
    );

    /// @brief Gets a cached AtlasMap by key.
    /// @param key Unique key.
    /// @return Raw pointer to AtlasMap or nullptr.
    static AtlasMap*
    getAtlas(const std::string& key);

    /// @brief Unloads all assets matching the specified scope across all
    /// managers (built-in and custom).
    /// @param scope Scope tag to clear.
    /// @return Total number of unloaded assets.
    static size_t
    unloadScope(const std::string& scope);

    /// @brief Clears all assets across all managers (built-in and custom).
    static void
    clear();

    /// @brief Polls hot reloading file checks across all managers (built-in and
    /// custom).
    static void
    checkHotReload();

    /// @brief Enables or disables hot reloading file checks across all managers
    /// (built-in and custom).
    /// @param enabled True to enable.
    static void
    setHotReloadEnabled(bool enabled);

private:
    /// @brief Private constructor for singleton pattern.
    AssetManager() = default;
    /// @brief Private destructor for singleton pattern.
    ~AssetManager() = default;

    /// @brief Manager for texture resources.
    ResourceManager<Texture> texture_manager;
    /// @brief Manager for shader resources.
    ResourceManager<Shader> shader_manager;
    /// @brief Manager for bitmap font resources.
    ResourceManager<BitmapFont> font_manager;
    /// @brief Manager for texture atlas resources.
    ResourceManager<AtlasMap> atlas_manager;

    /// @brief Registry for custom user-defined resource managers.
    std::unordered_map<std::type_index, std::unique_ptr<IResourceManager>>
        custom_managers;
};

template <typename T>
ResourceManager<T>&
AssetManager::getManager() {
    std::type_index type_idx(typeid(T));
    auto& custom_managers = get().custom_managers;
    auto it = custom_managers.find(type_idx);
    if (it != custom_managers.end()) {
        return *static_cast<ResourceManager<T>*>(it->second.get());
    }
    auto manager = std::make_unique<ResourceManager<T>>();
    ResourceManager<T>* ptr = manager.get();
    custom_managers[type_idx] = std::move(manager);
    return *ptr;
}

/// @brief Convenient shorthand alias for AssetManager.
using Assets = AssetManager;

}  // namespace lili
