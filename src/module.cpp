#include <fstream>
#include <filesystem>
#include <iostream>
#include <algorithm>
#include <cstring>

#include "module.h"

namespace wasm_rt
{

void Module::load(const std::string& file_name)
{
    m_valid = false;

    using namespace std;

    if (!filesystem::exists(file_name))
    {
        cerr << "File not found: " << file_name << endl;
        return;
    }
    const auto file_size = filesystem::file_size(file_name);
    if (file_size < 8)
    {
        cerr << "File too small: " << file_name << endl;
        return;
    }
    ifstream file(file_name, ios::binary);
    m_raw_data.clear();
    m_raw_data.resize(file_size);
    if (!file.read(reinterpret_cast<char*>(m_raw_data.data()), file_size))
    {
        cerr << "Failed to read file: " << file_name << endl;
        return;
    }

    check_valid();
}

void Module::check_valid()
{
    const uint8_t c_wasm_magic[] = {0x00, 0x61, 0x73, 0x6D}; // "\0asm"

    m_valid = false;
    if (m_raw_data.size() < 8
        || !std::equal(m_raw_data.begin(), m_raw_data.begin() + 4, c_wasm_magic))
    {
        return;
    }
    m_valid = true;
    m_version = 0;
    std::memcpy(&m_version, m_raw_data.data() + 4, 4);
}

} // namespace