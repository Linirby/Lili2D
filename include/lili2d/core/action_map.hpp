#pragma once

#include <set>
#include <string>
#include <unordered_map>
#include <utility>

#include "lili2d/core/keyboard.hpp"
#include "lili2d/core/keys.hpp"

namespace lili {

/// @brief Map action with custom set of keys
class ActionMap {
public:
    static ActionMap&
    get();

    void
    update();

    void
    add(const std::string& action, std::set<Key> keys = {});
    void
    remove(const std::string& action);

    void
    addKey(const std::string& action, Key key);
    void
    removeKey(const std::string& action, Key key);
    void
    replaceKey(const std::string& action, Key old_key, Key new_key);
    std::set<Key>
    getKeys(const std::string& action);

    bool
    isHeld(const std::string& action);
    bool
    isJustPressed(const std::string& action);
    bool
    isJustReleased(const std::string& action);

private:
    Keyboard keyboard;
    std::unordered_map<std::string, std::set<Key>> action_to_keys{};

    ActionMap() = default;
};

}  // namespace lili
