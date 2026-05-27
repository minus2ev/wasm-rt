#pragma once

#include <iostream>

#include "section.h"
#include "../utils/types.h"
#include "../utils/leb128.h"

namespace wasm_rt { namespace sections {

using namespace wasm_rt::utils;

class TypeSection : public Section
{
public:
    struct entry_t {
        Type type; // Func, or Empty if invalid
        std::vector<Type> params;
        std::vector<Type> returns;

        // for unit-tests
        bool operator==(const entry_t& other) const { return type == other.type && params == other.params && returns == other.returns; };
    };

    TypeSection(SectionId id, iter_t begin, iter_t end)
        : Section(id, begin, end)
    {
        if (id != SectionId::Type)
        {
            std::cerr << "Invalid section ID: " << static_cast<uint8_t>(id) << std::endl;
            return;
        }

        auto it = begin;
        const auto count = decode_leb128<uint32_t>(it, end);

        m_types.reserve(count);
        while (m_types.size() < count && it != end)
        {
            auto type = decode_leb128<Type>(it, end);
            if (type != Type::Func)
            {
                std::cerr << "Invalid type ID: " << static_cast<uint8_t>(type) << std::endl;
                type = Type::Empty; // treat as Empty
            }
            auto params = decode_vec<Type>(it, end);
            auto returns = decode_vec<Type>(it, end);
            m_types.push_back({
                type,
                params,
                returns
            });
        }
    }   
    virtual ~TypeSection() = default;
    const std::vector<entry_t>& types() const { return m_types; }
private:
    std::vector<entry_t> m_types;
};

}} // namespace wasm_rt::sections