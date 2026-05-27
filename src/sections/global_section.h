#pragma once

#include <iostream>

#include "section.h"
#include "../utils/types.h"
#include "../utils/leb128.h"

namespace wasm_rt { namespace sections {

using namespace wasm_rt::utils;

class GlobalSection : public Section
{
public:
    struct entry_t {
        Type type;
        uint8_t mut; // 0 for const, 1 for var
        std::vector<uint8_t> expr;
    };

    GlobalSection(SectionId id, iter_t begin, iter_t end)
        : Section(id, begin, end)
    {
        if (id != SectionId::Global)
        {
            std::cerr << "Invalid section ID: " << static_cast<uint8_t>(id) << std::endl;
            return;
        }

        auto it = begin;
        const auto count = decode_leb128<uint32_t>(it, end);

        m_globals.reserve(count);
        while (m_globals.size() < count && it != end)
        {
            auto type = static_cast<Type>(*it++); // type
            auto mut = *it++; // mutability
            auto iter = it; // store the beginning of the expression
            while (*it != 0x0b) // 0x0b is the end opcode for expression
            {
                // parse opcodes to skip the correct number of bytes
                switch (*it)
                {
                    case 0x41:
                    case 0x42:
                    case 0x23:
                    case 0xd2: {
                        decode_leb128<int64_t>(++it, end); // skip bytes, value not used
                        break;
                    }
                    case 0x43: {
                        it += 5;
                        break;
                    }
                    case 0x44: {
                        it += 9;
                        break;
                    }
                    case 0xd0: {
                        it += 2;
                        break;
                    }
                    default:
                        ++it;
                }
            }
            m_globals.push_back({
                type,
                mut,
                std::vector<uint8_t>(iter, it)
            });
            ++it; // skip 0x0b
        }
    }   
    virtual ~GlobalSection() = default;
    const std::vector<entry_t>& globals() const { return m_globals; }
private:
    std::vector<entry_t> m_globals;
};

}} // namespace wasm_rt::sections