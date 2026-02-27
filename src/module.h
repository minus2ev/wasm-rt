#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace wasm_rt {

class Module
{
public:
    void load(const std::string& file_name);
    bool is_valid() const { return m_valid; }
    int version() const { return m_version; }

private:
    void check_valid();

    bool m_valid = false;
    uint32_t m_version = 0;
    std::vector<uint8_t> m_raw_data;
};

} // namespace wasm_rt