#include "lili2d/core/string_hash.hpp"

namespace lili {

[[nodiscard]] std::size_t
StringHash::operator()(std::string_view sv) const noexcept {
    return std::hash<std::string_view>{}(sv);
}

}  // namespace lili
