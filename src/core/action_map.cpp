#include "lili2d/core/action_map.hpp"

#include <string>

#include "lili2d/core/event.hpp"
#include "lili2d/core/keyboard.hpp"

namespace lili {

ActionMap&
ActionMap::get() {
    static ActionMap instance = ActionMap();
    return instance;
}

void
ActionMap::update() {
    keyboard.update();
}

void
ActionMap::add(const std::string& action, std::set<Key> keys) {
    action_to_keys.insert_or_assign(action, std::move(keys));
}

void
ActionMap::remove(const std::string& action) {
    auto it = action_to_keys.find(action);
    if (it != action_to_keys.end()) action_to_keys.erase(it);
}

void
ActionMap::addKey(const std::string& action, Key key) {
    auto it = action_to_keys.find(action);
    if (it != action_to_keys.end()) action_to_keys[action].insert(key);
}

void
ActionMap::removeKey(const std::string& action, Key key) {
    auto it = action_to_keys.find(action);
    if (it != action_to_keys.end()) {
        auto key_it = action_to_keys[action].find(key);
        if (key_it != action_to_keys[action].end())
            action_to_keys[action].erase(key_it);
    }
}

void
ActionMap::replaceKey(const std::string& action, Key old_key, Key new_key) {
    if (old_key == new_key) return;

    auto it = action_to_keys.find(action);
    if (it != action_to_keys.end()) {
        auto key_it = action_to_keys[action].find(old_key);
        action_to_keys[action].erase(key_it);
        action_to_keys[action].insert(new_key);
    }
}

std::set<Key>
ActionMap::getKeys(const std::string& action) {
    auto it = action_to_keys.find(action);
    if (it != action_to_keys.end()) return action_to_keys[action];
    return {};
}

bool
ActionMap::isHeld(const std::string& action) {
    auto it = action_to_keys.find(action);
    if (it != action_to_keys.end())
        for (Key key : action_to_keys[action])
            if (keyboard.held(static_cast<Scancode>(key))) return true;
    return false;
}

bool
ActionMap::isJustPressed(const std::string& action) {
    auto it = action_to_keys.find(action);
    if (it != action_to_keys.end())
        for (Key key : action_to_keys[action])
            if (keyboard.justPressed(static_cast<Scancode>(key))) return true;
    return false;
}

bool
ActionMap::isJustReleased(const std::string& action) {
    auto it = action_to_keys.find(action);
    if (it != action_to_keys.end())
        for (Key key : action_to_keys[action])
            if (keyboard.justReleased(static_cast<Scancode>(key))) return true;
    return false;
}

}  // namespace lili
