#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "lili2d/core/event.hpp"
#include "lili2d/core/keyboard.hpp"
#include "lili2d/core/keys.hpp"
#include "lili2d/core/mouse.hpp"
#include "lili2d/core/string_hash.hpp"

namespace lili {

struct ActionBinding {
    std::vector<Key> keys;
    std::vector<MouseButton> mouse_buttons;
};

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
    /// @brief Clear all registered actions.
    void
    clear();

    /// @brief Check if an action exist in the ActionMap.
    /// @param action The name of the action.
    /// @return bool `true`: the action exist, `false` the action doesn't exist
    [[nodiscard]] bool
    has(std::string_view action) const noexcept;
    /// @brief Add a new action to the ActionMap. The action is overwrites if
    /// it already exists.
    /// @param action The name of the action.
    /// @param keys The set of keys for the action (default: empty).
    /// @param mouse_btn The set of mouse buttons for the action (default:
    /// empty)
    /// @return bool `true`: action is added, `false`: action is overwritten
    ///
    /// Keys and MouseButtons vectors will be move in to the action map.
    bool
    add(const std::string& action, std::vector<Key> keys = {},
        std::vector<MouseButton> mouse_btn = {});
    /// @brief Remove an action from the ActionMap.
    /// @param action The name of the action.
    /// @return bool `true`: action is removed, `false`: action doesn't exist
    bool
    remove(std::string_view action);

    /// @brief Add a key to an existing action.
    /// @param action The name of the action.
    /// @param key The key to add to the action.
    /// @return bool `true`: the key added to action, `false`: action doesn't
    /// exist
    bool
    addKey(std::string_view action, Key key);
    /// @brief Remove a key from an existing action.
    /// @param action The name of the action.
    /// @param key The key to remove from the action.
    /// @return bool `true`: the key is removed, `false`: action or key doesn't
    /// exist
    bool
    removeKey(std::string_view action, Key key);
    /// @brief Replace an existing key of an action with a new key.
    /// @param action The name of the action.
    /// @param old_key The key to be replaced.
    /// @param new_key The new key to replace with.
    /// @return bool `true`: key is replaced, `false`: action or key doesn't
    /// exist
    bool
    replaceKey(std::string_view action, Key old_key, Key new_key);
    /// @brief Get the set of keys for an existing action.
    /// @param action The name of the action.
    /// @return const std::vector<Key>& Const reference to a vector of keys for
    /// the corresponding action (empty set if action doesn't exist).
    [[nodiscard]] const std::vector<Key>&
    getKeys(std::string_view action) const noexcept;

    /// @brief Add a mouse button to an existing action.
    /// @param action The name of the action.
    /// @param btn The mouse button to add to the action.
    /// @return bool `true`: the mouse button added to action, `false`: action
    /// doesn't exist
    bool
    addMouseButton(std::string_view action, MouseButton btn);
    /// @brief Remove a mouse button from an existing action.
    /// @param action The name of the action.
    /// @param btn The mouse button to remove from the action.
    /// @return bool `true`: the mouse button is removed, `false`: action or
    /// mouse button doesn't exist
    bool
    removeMouseButton(std::string_view action, MouseButton btn);
    /// @brief Replace an existing mouse button of an action with a new
    /// mouse button.
    /// @param action The name of the action.
    /// @param old_btn The mouse button to be replmouse_btnaced.
    /// @param new_btn The new mouse button to replace with.
    /// @return bool `true`: mouse button is replaced, `false`: action or
    /// mouse button doesn't exist
    bool
    replaceMouseButton(
        std::string_view action, MouseButton old_btn, MouseButton new_btn
    );
    /// @brief Get the set of mouse button for an existing action.
    /// @param action The name of the action.
    /// @return const std::vector<MouseButton>& Const reference to a vector of
    /// keys for the corresponding action (empty set if action doesn't exist).
    [[nodiscard]] const std::vector<MouseButton>&
    getMouseButtons(std::string_view action) const noexcept;

    /// @brief Check if an action is currently held down.
    /// @param action The name of the action.
    /// @return bool `true`: action is held down, `false`: action is not held
    /// down or doesn't exist
    [[nodiscard]] bool
    isHeld(std::string_view action) const noexcept;
    /// @brief Check if an action was just pressed in the current frame.
    /// @param action The name of the action.
    /// @return bool `true`: action is just pressed, `false`: action is not
    /// just pressed or doesn't exist
    [[nodiscard]] bool
    isJustPressed(std::string_view action) const noexcept;
    /// @brief Check if an action was just released in the current frame.
    /// @param action The name of the action.
    /// @return bool `true`: action is just released, `false`: action is not
    /// just released or doesn't exist
    [[nodiscard]] bool
    isJustReleased(std::string_view action) const noexcept;

private:
    Keyboard keyboard;
    Mouse mouse;
    StringMap<ActionBinding> actions{};

    ActionMap() = default;
};

}  // namespace lili
