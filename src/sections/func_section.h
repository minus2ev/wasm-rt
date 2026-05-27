#pragma once

#include <iostream>

#include "section.h"
#include "../utils/types.h"
#include "../utils/leb128.h"

namespace wasm_rt { namespace sections {

using namespace wasm_rt::utils;

class FuncSection : public Section
{
public:
    struct entry_t {
        uint32_t type_index;
    };

    FuncSection(SectionId id, iter_t begin, iter_t end)
        : Section(id, begin, end)
    {
        if (id != SectionId::Function)
        {
            std::cerr << "Invalid section ID: " << static_cast<uint8_t>(id) << std::endl;
            return;
        }

        auto it = begin;
        const auto count = decode_leb128<uint32_t>(it, end);

        m_funcs.reserve(count);
        while (m_funcs.size() < count && it != end)
        {
            m_funcs.push_back({decode_leb128<uint32_t>(it, end)});
        }
    }   
    virtual ~FuncSection() = default;
    const std::vector<entry_t>& funcs() const { return m_funcs; }
private:
    std::vector<entry_t> m_funcs;
};

}} // namespace wasm_rt::sections