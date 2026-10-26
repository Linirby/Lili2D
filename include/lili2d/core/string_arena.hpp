#pragma once

#include <cstdint>
#include <string_view>
#include <vector>

namespace lili {

struct FreeSpace
{
    uint32_t offset;
    uint32_t length;
};

class StringArena
{
public:
    void
    insert(std::string_view text, uint32_t offset, uint32_t length);

    /// @brief Append a text to the content of the StringArena.
    /// @param text The string you want to add.
    /// @return The offset of where the text is add.
    uint32_t
    append(std::string_view text);

    void
    free(uint32_t offset, uint32_t length)
    {
        if (length > 0)
            free_spaces.push_back({ offset, length + 1 });
    }

    void
    reserve(uint32_t char_capacity, uint32_t free_capacity = 32)
    {
        content.reserve(char_capacity);
        free_spaces.reserve(free_capacity);
    }

    [[nodiscard]] std::string_view
    getStringView(uint32_t offset, uint32_t length) const noexcept
    {
        if (content.empty() || offset + length > content.size())
            return {};
        return std::string_view(content.data() + offset, length);
    }

    [[nodiscard]] const std::vector<char>&
    getContent() const noexcept
    {
        return content;
    }

    [[nodiscard]] std::vector<char>&
    getContent() noexcept
    {
        return content;
    }

private:
    static constexpr uint32_t MIN_FREE_LENGTH = 8;

    std::vector<char> content;
    std::vector<FreeSpace> free_spaces;
};

} // namespace lili
