#pragma once

#include <iostream>

#include "section.h"
#include "../utils/types.h"
#include "../utils/leb128.h"

namespace wasm_rt { namespace sections {

using namespace wasm_rt::utils;

class ExportSection : public Section
{
public:
    struct entry_t {
        std::vector<uint8_t> name;
        uint8_t type;
        uint32_t index;
    };

    ExportSection(SectionId id, iter_t begin, iter_t end)
        : Section(id, begin, end)
    {
        if (id != SectionId::Export)
        {
            std::cerr << "Invalid section ID: " << static_cast<uint8_t>(id) << std::endl;
            return;
        }

        auto it = begin;
        const auto count = decode_leb128<uint32_t>(it, end);

        m_exports.reserve(count);
        while (m_exports.size() < count && it != end)
        {
            const auto name_len = decode_leb128<uint32_t>(it, end);
            const auto name = std::vector<uint8_t>(it, it + name_len);
            it += name_len;
            const auto type = *it++;
            const auto index = decode_leb128<uint32_t>(it, end);
            m_exports.push_back({
                std::move(name),
                type,
                index
            });
        }
    }   
    virtual ~ExportSection() = default;
    const std::vector<entry_t>& exports() const { return m_exports; }
private:
    std::vector<entry_t> m_exports;
};

}} // namespace wasm_rt::sections