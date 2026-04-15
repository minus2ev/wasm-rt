#pragma once

#include <cstdint>

namespace wasm_rt { namespace utils {

enum class Type : uint8_t {
    Empty = 0x40,

    // Composite
    Func = 0x60,

    // Value
    I32 = 0x7f,
    I64 = 0x7e,
    F32 = 0x7d,
    F64 = 0x7c,
    V128 = 0x7b,

    // Reference
    FuncRef = 0x70,
    ExternRef = 0x6f,
};

}} // namespace wasm_rt::utils