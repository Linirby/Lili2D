#include "lili2d/core/action_map.hpp"

#include <string>

#include "lili2d/core/event.hpp"
#include "lili2d/core/keyboard.hpp"
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

bool
ActionMap::has(const std::string& action) {
    return action_to_keys.contains(action) ||
           action_to_mouse_btn.contains(action);
}

bool
ActionMap::add(
    const std::string& action, std::set<Key> keys,
    std::set<MouseButton> mouse_btn
) {
    bool res = false;
    res = action_to_keys.insert_or_assign(action, std::move(keys)).second;
    res = action_to_mouse_btn.insert_or_assign(action, std::move(mouse_btn))
              .second;
    return res;
}

bool
ActionMap::remove(const std::string& action) {
    bool res = false;
    auto key_it = action_to_keys.find(action);
    if (key_it != action_to_keys.end()) {
        action_to_keys.erase(key_it);
        res = true;
    }
    auto mouse_btn_it = action_to_mouse_btn.find(action);
    if (mouse_btn_it != action_to_mouse_btn.end()) {
        action_to_mouse_btn.erase(mouse_btn_it);
        res = true;
    }
    return res;
}

bool
ActionMap::addKey(const std::string& action, Key key) {
    if (action_to_keys.contains(action)) {
        action_to_keys[action].insert(key);
        return true;
    }
    return false;
}

bool
ActionMap::removeKey(const std::string& action, Key key) {
    if (action_to_keys.contains(action)) {
        auto key_it = action_to_keys[action].find(key);
        if (key_it != action_to_keys[action].end()) {
            action_to_keys[action].erase(key_it);
            return true;
        }
    }
    return false;
}

bool
ActionMap::replaceKey(const std::string& action, Key old_key, Key new_key) {
    if (action_to_keys.contains(action)) {
        auto key_it = action_to_keys[action].find(old_key);
        if (key_it != action_to_keys[action].end()) {
            if (old_key != new_key) {
                action_to_keys[action].erase(key_it);
                action_to_keys[action].insert(new_key);
            }
            return true;
        }
    }
    return false;
}

std::set<Key>
ActionMap::getKeys(const std::string& action) {
    if (action_to_keys.contains(action)) return action_to_keys[action];
    return {};
}

bool
ActionMap::addMouseButton(const std::string& action, MouseButton mouse_btn) {
    if (action_to_mouse_btn.contains(action)) {
        action_to_mouse_btn[action].insert(mouse_btn);
        return true;
    }
    return false;
}

bool
ActionMap::removeMouseButton(const std::string& action, MouseButton mouse_btn) {
    if (action_to_mouse_btn.contains(action)) {
        auto btn_it = action_to_mouse_btn[action].find(mouse_btn);
        if (btn_it != action_to_mouse_btn[action].end()) {
            action_to_mouse_btn[action].erase(btn_it);
            return true;
        }
    }
    return false;
}

bool
ActionMap::replaceMouseButton(
    const std::string& action, MouseButton old_mouse_btn,
    MouseButton new_mouse_btn
) {
    if (action_to_mouse_btn.contains(action)) {
        auto btn_it = action_to_mouse_btn[action].find(old_mouse_btn);
        if (btn_it != action_to_mouse_btn[action].end()) {
            if (old_mouse_btn != new_mouse_btn) {
                action_to_mouse_btn[action].erase(btn_it);
                action_to_mouse_btn[action].insert(new_mouse_btn);
            }
            return true;
        }
    }
    return false;
}

std::set<MouseButton>
ActionMap::getMouseButtons(const std::string& action) {
    if (action_to_mouse_btn.contains(action))
        return action_to_mouse_btn[action];
    return {};
}

bool
ActionMap::isHeld(const std::string& action) {
    if (action_to_keys.contains(action))
        for (Key key : action_to_keys[action])
            if (keyboard.held(static_cast<Scancode>(key))) return true;

    if (action_to_mouse_btn.contains(action))
        for (MouseButton btn : action_to_mouse_btn[action])
            if (mouse.held(btn)) return true;

    return false;
}

bool
ActionMap::isJustPressed(const std::string& action) {
    if (action_to_keys.contains(action))
        for (Key key : action_to_keys[action])
            if (keyboard.justPressed(static_cast<Scancode>(key))) return true;

    if (action_to_mouse_btn.contains(action))
        for (MouseButton btn : action_to_mouse_btn[action])
            if (mouse.justPressed(btn)) return true;

    return false;
}

bool
ActionMap::isJustReleased(const std::string& action) {
    if (action_to_keys.contains(action))
        for (Key key : action_to_keys[action])
            if (keyboard.justReleased(static_cast<Scancode>(key))) return true;

    if (action_to_mouse_btn.contains(action))
        for (MouseButton btn : action_to_mouse_btn[action])
            if (mouse.justReleased(btn)) return true;

    return false;
}

}  // namespace lili
