#pragma once

#include <vector>
#include <cstdint>
#include <type_traits>

namespace wasm_rt { namespace utils {

using iter_t = std::vector<uint8_t>::const_iterator;

template <typename T>
typename std::enable_if_t<std::is_integral_v<T>, T>
decode_leb128(iter_t& it, iter_t end)
{
    if (it == end)
    {
        return T{};
    }

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
    return value;
}

template <typename T>
typename std::enable_if_t<std::is_enum_v<T>, T>
decode_leb128(iter_t& it, iter_t end)
{
    using U = typename std::underlying_type_t<T>;
    return static_cast<T>(decode_leb128<U>(it, end));
}

template <typename T>
std::vector<T> decode_vec(iter_t& it, iter_t end)
{
    std::vector<T> result;
    const auto count = decode_leb128<uint32_t>(it, end);
    result.reserve(count);
    while (it != end && result.size() < count)
    {
        const auto value = decode_leb128<T>(it, end);
        result.push_back(value);
    }
    return std::move(result);
}

}} // namespace wasm_rt::utils