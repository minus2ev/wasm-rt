#pragma once

#include <iostream>

#include "section.h"
#include "../utils/types.h"
#include "../utils/leb128.h"

namespace wasm_rt { namespace sections {

using namespace wasm_rt::utils;

class MemSection : public Section
{
public:
    struct entry_t {
        uint8_t has_max; // 0 for min, 1 for min and max
        uint32_t min;
        uint32_t max;
    };

    MemSection(SectionId id, iter_t begin, iter_t end)
        : Section(id, begin, end)
    {
        if (id != SectionId::Memory)
        {
            std::cerr << "Invalid section ID: " << static_cast<uint8_t>(id) << std::endl;
            return;
        }

        auto it = begin;
        const auto count = decode_leb128<uint32_t>(it, end);

        m_mems.reserve(count);
        while (m_mems.size() < count && it != end)
        {
            auto flag = *it++;
            auto min = decode_leb128<uint32_t>(it, end);
            auto max = (flag & 0x1) ? decode_leb128<uint32_t>(it, end) : 0;
            m_mems.push_back({
                flag,
                min,
                max
            });
        }
    }   
    virtual ~MemSection() = default;
    const std::vector<entry_t>& mems() const { return m_mems; }
private:
    std::vector<entry_t> m_mems;
};

}} // namespace wasm_rt::sections