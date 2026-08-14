#pragma once

#include <set>
#include <string>
#include <unordered_map>
#include <utility>

#include "lili2d/core/event.hpp"
#include "lili2d/core/keyboard.hpp"
#include "lili2d/core/keys.hpp"
#include "lili2d/core/mouse.hpp"

namespace lili {

/// @brief Map action with custom set of keys
class ActionMap {
public:
    /// @brief Delete copy constructor.
    ActionMap(const ActionMap&) = delete;
    /// @brief Delete copy assignment operator.
    ActionMap&
    operator=(const ActionMap&) = delete;

    /// @brief Gets the singleton instance.
    /// @return Reference to the ActionMap.
    static ActionMap&
    get();

    /// @brief Update all the input devices (Keyboard and Mouse now).
    void
    update();

    /// @brief Check if an action exist in the ActionMap.
    /// @param action The name of the action.
    /// @return bool `true`: the action exist, `false` the action doesn't exist
    bool
    has(const std::string& action) const;
    /// @brief Add a new action to the ActionMap. The action is overwrites if
    /// it already exists.
    /// @param action The name of the action.
    /// @param keys The set of keys for the action (default: empty).
    /// @param mouse_btn The set of mouse buttons for the action (default:
    /// empty)
    /// @return bool `true`: action is added, `false`: action is overwritten
    bool
    add(const std::string& action, std::set<Key> keys = {},
        std::set<MouseButton> mouse_btn = {});
    /// @brief Remove an action from the ActionMap.
    /// @param action The name of the action.
    /// @return bool `true`: action is removed, `false`: action doesn't exist
    bool
    remove(const std::string& action);

    /// @brief Add a key to an existing action.
    /// @param action The name of the action.
    /// @param key The key to add to the action.
    /// @return bool `true`: the key added to action, `false`: action doesn't
    /// exist
    bool
    addKey(const std::string& action, Key key);
    /// @brief Remove a key from an existing action.
    /// @param action The name of the action.
    /// @param key The key to remove from the action.
    /// @return bool `true`: the key is removed, `false`: action or key doesn't
    /// exist
    bool
    removeKey(const std::string& action, Key key);
    /// @brief Replace an existing key of an action with a new key.
    /// @param action The name of the action.
    /// @param old_key The key to be replaced.
    /// @param new_key The new key to replace with.
    /// @return bool `true`: key is replaced, `false`: action or key doesn't
    /// exist
    bool
    replaceKey(const std::string& action, Key old_key, Key new_key);
    /// @brief Get the set of keys for an existing action.
    /// @param action The name of the action.
    /// @return std::set<Key> Set of keys for the action (empty set if action
    /// doesn't exist).
    std::set<Key>
    getKeys(const std::string& action) const;

    /// @brief Add a mouse button to an existing action.
    /// @param action The name of the action.
    /// @param mouse_btn The mouse button to add to the action.
    /// @return bool `true`: the mouse button added to action, `false`: action
    /// doesn't exist
    bool
    addMouseButton(const std::string& action, MouseButton mouse_btn);
    /// @brief Remove a mouse button from an existing action.
    /// @param action The name of the action.
    /// @param mouse_btn The mouse button to remove from the action.
    /// @return bool `true`: the mouse button is removed, `false`: action or
    /// mouse button doesn't exist
    bool
    removeMouseButton(const std::string& action, MouseButton mouse_btn);
    /// @brief Replace an existing mouse button of an action with a new
    /// mouse button.
    /// @param action The name of the action.
    /// @param old_mouse_btn The mouse button to be replaced.
    /// @param new_mouse_btn The new mouse button to replace with.
    /// @return bool `true`: mouse button is replaced, `false`: action or
    /// mouse button doesn't exist
    bool
    replaceMouseButton(
        const std::string& action, MouseButton old_mouse_btn,
        MouseButton new_mouse_btn
    );
    /// @brief Get the set of mouse button for an existing action.
    /// @param action The name of the action.
    /// @return std::set<MouseButton> Set of mouse button for the action (empty
    /// set if action doesn't exist).
    std::set<MouseButton>
    getMouseButtons(const std::string& action) const;

    /// @brief Check if an action is currently held down.
    /// @param action The name of the action.
    /// @return bool `true`: action is held down, `false`: action is not held
    /// down or doesn't exist
    bool
    isHeld(const std::string& action) const;
    /// @brief Check if an action was just pressed in the current frame.
    /// @param action The name of the action.
    /// @return bool `true`: action is just pressed, `false`: action is not
    /// just pressed or doesn't exist
    bool
    isJustPressed(const std::string& action) const;
    /// @brief Check if an action was just released in the current frame.
    /// @param action The name of the action.
    /// @return bool `true`: action is just released, `false`: action is not
    /// just released or doesn't exist
    bool
    isJustReleased(const std::string& action) const;

private:
    Keyboard keyboard;
    Mouse mouse;

    std::unordered_map<std::string, std::set<Key>> action_to_keys{};
    std::unordered_map<std::string, std::set<MouseButton>>
        action_to_mouse_btn{};

    ActionMap() = default;
};

}  // namespace lili
