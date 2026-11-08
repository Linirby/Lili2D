#pragma once

#include "lili2d/components/text.hpp"
#include <cstdint>
#include <format>
#include <string_view>
#include <utility>
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
    /// @brief Append a text to the content of the StringArena.
    /// @param text The string you want to add.
    /// @return the offset of the string in the arena.
    uint32_t
    append(std::string_view text, uint32_t capacity = 0);
    /// @brief Append a text to the content of the StringArena.
    /// @param text The string you want to add.
    /// @param comp The TextComponent where offset, length, capacity are stored
    void
    append(std::string_view text, TextComponent& comp)
    {
        comp.offset = append(text, comp.capacity);
        comp.length = text.size();
        if (static_cast<uint32_t>(text.size()) > comp.capacity)
            comp.capacity = static_cast<uint32_t>(text.size());
    }

    /// @brief Formats a string directly into a pre-allocated slot in the arena.
    /// @note The number of characters written is the new length of the text.
    /// @param offset The starting index in the arena.
    /// @param max_capacity Max number of characters to write (excluding '\0').
    /// @param fmt The std::format format string.
    /// @param args Arguments to format.
    /// @return The length of the formatted.
    template<typename... Args>
    uint32_t
    formatAt(
        uint32_t offset,
        uint32_t capacity,
        std::format_string<Args...> fmt,
        Args&&... args
    )
    {
        if (offset >= content.size())
            return 0;

        uint32_t available = static_cast<uint32_t>(content.size() - 1 - offset);
        uint32_t limit =
            (capacity <= 0) ? available : std::min(capacity, available);

        auto [out, count] = std::format_to_n(
            content.data() + offset, limit, fmt, std::forward<Args>(args)...
        );

        uint32_t len = static_cast<uint32_t>(out - (content.data() + offset));
        content[offset + len] = '\0';
        return len;
    }
    /// @brief Formats a string directly into a pre-allocated slot in the arena.
    /// @note The number of characters written is the new length of the text.
    /// @param offset The starting index in the arena.
    /// @param max_capacity Max number of characters to write (excluding '\0').
    /// @param fmt The std::format format string.
    /// @param args Arguments to format.
    template<typename... Args>
    void
    formatAt(
        TextComponent& comp,
        std::format_string<Args...> fmt,
        Args&&... args
    )
    {
        comp.length = formatAt(
            comp.offset, comp.capacity, fmt, std::forward<Args>(args)...
        );
    }

    void
    free(uint32_t offset, uint32_t capacity)
    {
        if (capacity > 0)
            free_spaces.push_back({ offset, capacity + 1 });
    }

    void
    free(TextComponent comp)
    {
        free(comp.offset, comp.capacity);
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

    [[nodiscard]] std::string_view
    getStringView(TextComponent comp) const noexcept
    {
        return getStringView(comp.offset, comp.length);
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
