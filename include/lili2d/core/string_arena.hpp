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
    /// @brief Insert a text to the content of the StringArena at a specific
    /// offset with a specific length
    /// @param text The string you want to add.
    /// @param offset The offset where you start insertion.
    /// @param length The length of the insertion.
    void
    insert(std::string_view text, uint32_t offset, uint32_t length);
    /// @brief Insert a text to the content of the StringArena at a specific
    /// offset with a specific length
    /// @param text The string you want to add.
    /// @param offset The offset where you start insertion.
    /// @param length The length of the insertion.
    void
    insert(std::string_view text, TextComponent comp)
    {
        insert(text, comp.offset, comp.length);
    }

    /// @brief Append a text to the content of the StringArena.
    /// @param text The string you want to add.
    /// @return the offset of the string in the arena.
    uint32_t
    append(std::string_view text);
    /// @brief Append a text to the content of the StringArena.
    /// @param text The string you want to add.
    /// @param comp The TextComponent where offset, length will be stored
    void
    append(std::string_view text, TextComponent& comp)
    {
        comp.offset = append(text);
        comp.length = text.size();
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
        uint32_t max_capacity,
        std::format_string<Args...> fmt,
        Args&&... args
    )
    {
        if (offset >= content.size())
            return 0;

        uint32_t available = static_cast<uint32_t>(content.size() - 1 - offset);
        uint32_t limit =
            (max_capacity <= 0) ? available : std::min(max_capacity, available);

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
        uint32_t max_capacity,
        std::format_string<Args...> fmt,
        Args&&... args
    )
    {
        comp.length = formatAt(
            comp.offset, max_capacity, fmt, std::forward<Args>(args)...
        );
    }

    void
    free(uint32_t offset, uint32_t length)
    {
        if (length > 0)
            free_spaces.push_back({ offset, length + 1 });
    }

    void
    free(TextComponent comp)
    {
        free(comp.offset, comp.length);
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
