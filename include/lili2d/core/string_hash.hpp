#pragma once

#include <cstddef>
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>
namespace lili {

struct StringHash {
    using is_transparent = void;

    [[nodiscard]] std::size_t
    operator()(std::string_view sv) const noexcept;
};

template <typename T>
using StringMap =
    std::unordered_map<std::string, T, StringHash, std::equal_to<>>;

}  // namespace lili
