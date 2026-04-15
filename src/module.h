#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <functional>
#include <optional>
#include <memory>

#include "sections/section.h"

namespace wasm_rt {

class Module
{
public:
    void load(const std::string& file_name);
    bool is_valid() const { return m_valid; }
    int version() const { return m_version; }
    std::optional<std::reference_wrapper<sections::Section>> section(sections::SectionId id) const;

private:
    bool m_valid{false};
    uint32_t m_version{0};
    std::vector<std::unique_ptr<sections::Section>> m_sections;
};

} // namespace wasm_rt