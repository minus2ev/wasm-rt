#pragma once

#include <iostream>

#include "section.h"
#include "../utils/types.h"
#include "../utils/leb128.h"

namespace wasm_rt { namespace sections {

using namespace wasm_rt::utils;

class CodeSection : public Section
{
public:
    using entry_t = std::tuple<
        std::vector<std::tuple< // local variables
            Type,               // type
            uint32_t            // count
        >>,
        std::vector<uint8_t>    // expression
    >;

    CodeSection(SectionId id, iter_t begin, iter_t end)
        : Section(id, begin, end)
    {
        if (id != SectionId::Code)
        {
            std::cerr << "Invalid section ID: " << static_cast<uint8_t>(id) << std::endl;
            return;
        }

        auto it = begin;
        const auto count = decode_leb128<uint32_t>(it, end);

        m_code.reserve(count);
        while (m_code.size() < count && it != end)
        {
            auto func_start = it;
            const auto size = decode_leb128<uint32_t>(it, end);
            std::vector<std::tuple<Type, uint32_t>> locals;
            const auto local_count = decode_leb128<uint32_t>(it, end);
            locals.reserve(local_count);
            for (uint32_t i = 0; i < local_count && it != end; ++i)
            {
                const auto count = decode_leb128<uint32_t>(it, end);
                const auto type = decode_leb128<Type>(it, end);
                locals.push_back({ type, count });
            }
            auto expr = std::vector<uint8_t>(it, func_start + size);
            m_code.push_back({
                std::move(locals),
                std::move(expr)
            });
        }
    }   
    virtual ~CodeSection() = default;
    const std::vector<entry_t>& code() const { return m_code; }
private:
    std::vector<entry_t> m_code;
};

}} // namespace wasm_rt::sections