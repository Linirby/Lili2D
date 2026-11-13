#pragma once

#include "lili2d/components/text.hpp"
#include <cstdint>
#include <format>
#include <string_view>
#include <utility>
#include <vector>

namespace lili {

/// @brief Represents a freed contiguous block in the StringArena.
struct FreeSpace
{
    uint32_t offset; ///< Starting character index in the arena.
    uint32_t length; ///< Length of the free block in bytes.
};

/// @brief Contiguous character arena for zero-allocation ECS text storage.
class StringArena
{
public:
    /// @brief Append a text to the content of the StringArena.
    /// @param text The string you want to add.
    /// @param capacity Optional pre-allocated capacity (0 = text.size()).
    /// @return the offset of the string in the arena.
    uint32_t
    append(std::string_view text, uint32_t capacity = 0);
    /// @brief Append a text to the content of the StringArena.
    /// @param text The string you want to add.
    /// @param comp The cText where offset, length, capacity are stored
    void
    append(std::string_view text, cText& comp)
    {
        comp.offset = append(text, comp.capacity);
        comp.length = text.size();
        if (static_cast<uint32_t>(text.size()) > comp.capacity)
            comp.capacity = static_cast<uint32_t>(text.size());
    }

    /// @brief Formats a string directly into a pre-allocated slot in the arena.
    /// @note The number of characters written is the new length of the text.
    /// @param offset The starting index in the arena.
    /// @param capacity Max number of characters to write (excluding '\0').
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
    /// @param comp The cText whose slot will be formatted in-place.
    /// @param fmt The std::format format string.
    /// @param args Arguments to format.
    template<typename... Args>
    void
    formatAt(cText& comp, std::format_string<Args...> fmt, Args&&... args)
    {
        comp.length = formatAt(
            comp.offset, comp.capacity, fmt, std::forward<Args>(args)...
        );
    }

    /// @brief Marks a slice in the arena as free for future reuse.
    /// @param offset Starting index in the arena.
    /// @param capacity Allocated capacity (excluding null terminator).
    void
    free(uint32_t offset, uint32_t capacity)
    {
        if (capacity > 0)
            free_spaces.push_back({ offset, capacity + 1 });
    }

    /// @brief Marks the slice used by a cText as free for future reuse.
    /// @param comp The cText whose slice will be freed.
    void
    free(cText comp)
    {
        free(comp.offset, comp.capacity);
    }

    /// @brief Reserves capacity for the character buffer and free-space list.
    /// @param char_capacity Number of characters to reserve.
    /// @param free_capacity Number of free space entries to reserve.
    void
    reserve(uint32_t char_capacity, uint32_t free_capacity = 32)
    {
        content.reserve(char_capacity);
        free_spaces.reserve(free_capacity);
    }

    /// @brief Gets a string_view into the arena at the given offset and length.
    /// @param offset Starting index in the arena.
    /// @param length String length in characters.
    /// @return A string_view of the slice, or empty if out of bounds.
    [[nodiscard]] std::string_view
    getStringView(uint32_t offset, uint32_t length) const noexcept
    {
        if (content.empty() || offset + length > content.size())
            return {};
        return std::string_view(content.data() + offset, length);
    }

    /// @brief Gets a string_view for the text referenced by a cText.
    /// @param comp The cText referencing the slice.
    /// @return A string_view of the component's text.
    [[nodiscard]] std::string_view
    getStringView(cText comp) const noexcept
    {
        return getStringView(comp.offset, comp.length);
    }

    /// @brief Gets a const reference to the underlying character vector.
    /// @return Const reference to the character buffer.
    [[nodiscard]] const std::vector<char>&
    getContent() const noexcept
    {
        return content;
    }

    /// @brief Gets a mutable reference to the underlying character vector.
    /// @return Mutable reference to the character buffer.
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
