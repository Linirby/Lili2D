#include "lili2d/core/action_map.hpp"

#include <algorithm>

#include "lili2d/core/event.hpp"
#include "lili2d/core/keyboard.hpp"
#include "lili2d/core/keys.hpp"
#include "lili2d/core/mouse.hpp"

namespace lili {

ActionMap&
ActionMap::get() {
    static ActionMap instance = ActionMap();
    return instance;
}

void
ActionMap::update() {
    keyboard.update();
    mouse.update();
}

void
ActionMap::clear() {
    actions.clear();
}

bool
ActionMap::has(std::string_view action) const noexcept {
    return actions.find(action) != actions.end();
}

bool
ActionMap::add(
    const std::string& action, std::vector<Key> keys,
    std::vector<MouseButton> mouse_btn
) {
    auto [it, inserted] =
        actions.insert_or_assign(action, ActionBinding{keys, mouse_btn});
    return inserted;
}

bool
ActionMap::remove(std::string_view action) {
    auto it = actions.find(action);
    if (it != actions.end()) {
        actions.erase(it);
        return true;
    }
    return false;
}

bool
ActionMap::addKey(std::string_view action, Key key) {
    auto it = actions.find(action);
    if (it == actions.end()) return false;

    auto& keys = it->second.keys;
    if (std::find(keys.begin(), keys.end(), key) != keys.end()) return false;
    keys.push_back(key);
    return true;
}

bool
ActionMap::removeKey(std::string_view action, Key key) {
    auto it = actions.find(action);
    if (it == actions.end()) return false;

    auto& keys = it->second.keys;
    auto key_it = std::find(keys.begin(), keys.end(), key);
    if (key_it == keys.end()) return false;
    keys.erase(key_it);
    return true;
}

bool
ActionMap::replaceKey(std::string_view action, Key old_key, Key new_key) {
    auto it = actions.find(action);
    if (it == actions.end()) return false;

    auto& keys = it->second.keys;
    auto key_it = std::find(keys.begin(), keys.end(), old_key);
    if (key_it == keys.end()) return false;
    *key_it = new_key;
    return true;
}

const std::vector<Key>&
ActionMap::getKeys(std::string_view action) const noexcept {
    static const std::vector<Key> empty{};
    auto it = actions.find(action);
    return (it != actions.end()) ? it->second.keys : empty;
}

bool
ActionMap::addMouseButton(std::string_view action, MouseButton btn) {
    auto it = actions.find(action);
    if (it == actions.end()) return false;

    auto& btns = it->second.mouse_buttons;
    if (std::find(btns.begin(), btns.end(), btn) != btns.end()) return false;
    btns.push_back(btn);
    return true;
}

bool
ActionMap::removeMouseButton(std::string_view action, MouseButton btn) {
    auto it = actions.find(action);
    if (it == actions.end()) return false;

    auto& btns = it->second.mouse_buttons;
    auto btn_it = std::find(btns.begin(), btns.end(), btn);
    if (btn_it == btns.end()) return false;
    btns.erase(btn_it);
    return true;
}

bool
ActionMap::replaceMouseButton(
    std::string_view action, MouseButton old_btn, MouseButton new_btn
) {
    auto it = actions.find(action);
    if (it == actions.end()) return false;

    auto& btns = it->second.mouse_buttons;
    auto btn_it = std::find(btns.begin(), btns.end(), old_btn);
    if (btn_it == btns.end()) return false;
    *btn_it = new_btn;
    return true;
}

const std::vector<MouseButton>&
ActionMap::getMouseButtons(std::string_view action) const noexcept {
    static const std::vector<MouseButton> empty{};
    auto it = actions.find(action);
    return (it != actions.end()) ? it->second.mouse_buttons : empty;
}

bool
ActionMap::isHeld(std::string_view action) const noexcept {
    auto it = actions.find(action);
    if (it == actions.end()) return false;

    for (Key key : it->second.keys)
        if (keyboard.held(static_cast<Scancode>(key))) return true;
    for (MouseButton btn : it->second.mouse_buttons)
        if (mouse.held(btn)) return true;
    return false;
}

bool
ActionMap::isJustPressed(std::string_view action) const noexcept {
    auto it = actions.find(action);
    if (it == actions.end()) return false;

    for (Key key : it->second.keys)
        if (keyboard.justPressed(static_cast<Scancode>(key))) return true;
    for (MouseButton btn : it->second.mouse_buttons)
        if (mouse.justPressed(btn)) return true;
    return false;
}

bool
ActionMap::isJustReleased(std::string_view action) const noexcept {
    auto it = actions.find(action);
    if (it == actions.end()) return false;

    for (Key key : it->second.keys)
        if (keyboard.justReleased(static_cast<Scancode>(key))) return true;
    for (MouseButton btn : it->second.mouse_buttons)
        if (mouse.justReleased(btn)) return true;
    return false;
}

}  // namespace lili
