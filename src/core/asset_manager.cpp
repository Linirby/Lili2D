#include "lili2d/core/asset_manager.hpp"

namespace lili {

AssetManager&
AssetManager::get() {
    static AssetManager instance;
    return instance;
}

ResourceManager<Texture>&
AssetManager::textures() {
    return texture_manager;
}

ResourceManager<Shader>&
AssetManager::shaders() {
    return shader_manager;
}

ResourceManager<BitmapFont>&
AssetManager::fonts() {
    return font_manager;
}

ResourceManager<AtlasMap>&
AssetManager::atlases() {
    return atlas_manager;
}

Texture*
AssetManager::loadTexture(
    const std::string& key, const std::string& path, SDL_GPUDevice* device,
    const std::string& scope
) {
    return get().textures().load(
        key, path,
        [device](const std::string& p) {
            return std::make_unique<Texture>(device, p);
        },
        scope
    );
}

Texture*
AssetManager::loadTexture(
    const std::string& path, SDL_GPUDevice* device, const std::string& scope
) {
    return loadTexture(path, path, device, scope);
}

Texture*
AssetManager::getTexture(std::string_view key) {
    return get().textures().get(key);
}

Shader*
AssetManager::loadShader(
    const std::string& key, const std::string& vertPath,
    const std::string& fragPath, SDL_GPUDevice* device, ShaderInfo vert_infos,
    ShaderInfo frag_infos, const std::string& scope
) {
    return get().shaders().load(
        key, vertPath,
        [device, vertPath, fragPath, vert_infos,
         frag_infos](const std::string&) {
            return std::make_unique<Shader>(
                device, vertPath, fragPath, vert_infos, frag_infos
            );
        },
        scope
    );
}

Shader*
AssetManager::getShader(std::string_view key) {
    return get().shaders().get(key);
}

BitmapFont*
AssetManager::loadFont(
    const std::string& key, Renderer* renderer, const std::string& path,
    uint8_t cols, uint8_t rows, const std::string& scope
) {
    return get().fonts().load(
        key, path,
        [renderer, cols, rows](const std::string& p) {
            return std::make_unique<BitmapFont>(renderer, p, cols, rows);
        },
        scope
    );
}

BitmapFont*
AssetManager::getFont(std::string_view key) {
    return get().fonts().get(key);
}

AtlasMap*
AssetManager::loadAtlas(
    const std::string& key, Renderer* renderer, const std::string& path,
    int cols, int rows, const std::string& scope
) {
    return get().atlases().load(
        key, path,
        [renderer, cols, rows](const std::string& p) {
            auto atlas = std::make_unique<AtlasMap>(renderer, p);
            atlas->slice(cols, rows);
            return atlas;
        },
        scope
    );
}

AtlasMap*
AssetManager::getAtlas(std::string_view key) {
    return get().atlases().get(key);
}

size_t
AssetManager::unloadScope(std::string_view scope) {
    size_t count = 0;
    count += get().textures().unloadScope(scope);
    count += get().shaders().unloadScope(scope);
    count += get().fonts().unloadScope(scope);
    count += get().atlases().unloadScope(scope);

    for (auto& [type_idx, mgr] : get().custom_managers)
        if (mgr) count += mgr->unloadScope(scope);
    return count;
}

void
AssetManager::clear() {
    get().textures().clear();
    get().shaders().clear();
    get().fonts().clear();
    get().atlases().clear();

    for (auto& [type_idx, mgr] : get().custom_managers)
        if (mgr) mgr->clear();
}

void
AssetManager::checkHotReload() {
    get().textures().checkHotReload();
    get().shaders().checkHotReload();
    get().fonts().checkHotReload();
    get().atlases().checkHotReload();

    for (auto& [type_idx, mgr] : get().custom_managers)
        if (mgr) mgr->checkHotReload();
}

void
AssetManager::setHotReloadEnabled(bool enabled) {
    get().textures().setHotReloadEnabled(enabled);
    get().shaders().setHotReloadEnabled(enabled);
    get().fonts().setHotReloadEnabled(enabled);
    get().atlases().setHotReloadEnabled(enabled);

    for (auto& [type_idx, mgr] : get().custom_managers)
        if (mgr) mgr->setHotReloadEnabled(enabled);
}

}  // namespace lili
