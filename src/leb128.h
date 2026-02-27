#pragma once

#include <vector>
#include <cstdint>
#include <type_traits>

namespace wasm_rt { namespace utils {

using iter_t = std::vector<uint8_t>::const_iterator;

template <typename T>
std::vector<T> decode_leb128(iter_t& it, iter_t end, size_t num_values)
{
    static_assert(std::is_integral_v<T> && !std::is_same_v<T, bool>, "T must be an integral type");
    if (it == end)
    {
        return {};
    }
    std::vector<T> result;
    result.reserve(num_values);
    for (size_t count = 0; count <= num_values; ++count)
    {
        T value = 0;
        size_t shift = 0;
        uint8_t byte;
        do
        {
            byte = *it;
            value |= (byte & 0x7F) << shift;
            shift += 7;
            ++it;
        } while ((byte & 0x80) != 0 && it != end);
        if constexpr (std::is_signed_v<T>)
        {
            if (shift < sizeof(T) * 8 && (byte & 0x40) != 0)
            {
                value |= ~0ull << shift;
            }
        }
        result.push_back(value);
        if (it == end)
        {
            break;
        }
    }
    return result;
}

template <typename T>
T decode_leb128(iter_t& it, iter_t end)
{
    return it != end ? decode_leb128<T>(it, end, 1)[0] : T{};
}

}} // namespace wasm_rt::utils