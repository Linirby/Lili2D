#pragma once

#include <cstddef>
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>

namespace lili {

/// @brief Transparent string hasher supporting std::string_view lookups.
struct StringHash
{
    /// @brief Type tag enabling heterogeneous lookup in associative containers.
    using is_transparent = void;

    /// @brief Computes hash for a string view.
    /// @param sv The string view to hash.
    /// @return The computed hash value.
    [[nodiscard]] std::size_t
    operator()(std::string_view sv) const noexcept
    {
        return std::hash<std::string_view>{}(sv);
    }
};

/// @brief Heterogeneous unordered_map keyed by std::string with transparent
/// std::string_view lookup.
/// @tparam T Value type stored in the map.
template<typename T>
using StringMap =
    std::unordered_map<std::string, T, StringHash, std::equal_to<>>;

} // namespace lili
