#pragma once

#include <cstdint>
#include <vector>

namespace wasm_rt { namespace sections {

using iter_t = std::vector<uint8_t>::const_iterator;

enum class SectionId : uint8_t
{
    Custom = 0,
    Type = 1,
    Import = 2,
    Function = 3,
    Table = 4,
    Memory = 5,
    Global = 6,
    Export = 7,
    Start = 8,
    Element = 9,
    Code = 10,
    Data = 11,
    DataCount = 12,
    Invalid = 0xFF
};

class Section
{
public:
    Section(SectionId id, iter_t begin, iter_t end)
        : m_id(id)
    {}
    virtual ~Section() = default;
    SectionId id() const { return m_id; }
private:
    SectionId m_id;
};

}} // namespace wasm_rt::sections