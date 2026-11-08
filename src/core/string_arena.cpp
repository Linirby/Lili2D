#include "lili2d/core/string_arena.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string_view>

namespace lili {

uint32_t
StringArena::append(std::string_view text, uint32_t capacity)
{
    const uint32_t size_cap =
        std::max(capacity, static_cast<uint32_t>(text.size()));
    const uint32_t needed_size = size_cap + 1;

    for (size_t i = 0; i < free_spaces.size(); ++i) {
        if (free_spaces[i].length < needed_size)
            continue;
        uint32_t offset = free_spaces[i].offset;
        if (!text.empty())
            std::memcpy(content.data() + offset, text.data(), text.size());
        content[offset + text.size()] = '\0';
        if (free_spaces[i].length >= needed_size + MIN_FREE_LENGTH) {
            free_spaces[i].offset += needed_size;
            free_spaces[i].length -= needed_size;
        } else {
            free_spaces[i] = free_spaces.back();
            free_spaces.pop_back();
        }
        return offset;
    }
    uint32_t offset = static_cast<uint32_t>(content.size());
    content.resize(offset + needed_size);
    if (!text.empty())
        std::memcpy(content.data() + offset, text.data(), text.size());
    content[offset + text.size()] = '\0';
    return offset;
}

} // namespace lili
