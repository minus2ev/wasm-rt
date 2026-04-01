#include <fstream>
#include <filesystem>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <map>

#include "module.h"
#include "utils/leb128.h"
#include "sections/type_section.h"
#include "sections/func_section.h"
#include "sections/mem_section.h"
#include "sections/global_section.h"

using namespace wasm_rt;
using namespace wasm_rt::sections;

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
    std::vector<uint8_t> raw_data;
    raw_data.clear();
    raw_data.resize(file_size);
    if (!file.read(reinterpret_cast<char*>(raw_data.data()), file_size))
    {
        cerr << "Failed to read file: " << file_name << endl;
        return;
    }

    // Check magic and version
    const uint8_t c_wasm_magic[] = {0x00, 0x61, 0x73, 0x6D}; // "\0asm"
    if (std::equal(raw_data.begin(), raw_data.begin() + 4, c_wasm_magic))
    {
        m_version = 0;
        std::memcpy(&m_version, raw_data.data() + 4, 4);
        if (m_version == 1)
        {
            m_valid = true;
        }
    }

    // Read sections
    const std::map<SectionId, std::function<std::unique_ptr<Section>(iter_t, iter_t)>> section_factories = {
        { SectionId::Type, [](iter_t it, iter_t end) { return std::make_unique<TypeSection>(SectionId::Type, it, end); } },
        { SectionId::Function, [](iter_t it, iter_t end) { return std::make_unique<FuncSection>(SectionId::Function, it, end); } },
        { SectionId::Memory, [](iter_t it, iter_t end) { return std::make_unique<MemSection>(SectionId::Memory, it, end); } },
        { SectionId::Global, [](iter_t it, iter_t end) { return std::make_unique<GlobalSection>(SectionId::Global, it, end); } }
    };
    auto it = raw_data.cbegin() + 8; // skip magic and version
    while (it != raw_data.cend())
    {
        const auto raw_id = *it++;
        const auto id = (raw_id >= 0 && raw_id <= static_cast<uint8_t>(SectionId::DataCount))
            ? static_cast<SectionId>(raw_id)
            : SectionId::Invalid;
        const auto size = utils::decode_leb128<uint32_t>(it, raw_data.cend());
        m_sections.push_back(section_factories.find(id) != section_factories.end()
            ? section_factories.at(id)(it, it + size)
            : std::make_unique<Section>(id, it, it + size));
        it += size;
    }
}

std::optional<std::reference_wrapper<Section>> Module::section(SectionId id) const
{
    auto it = std::find_if(m_sections.cbegin(), m_sections.cend(),
        [id](const auto& section) { return section->id() == id; });
    if (it != m_sections.cend())
    {
        return std::ref(**it);
    }
    else
    {
        std::cerr << "Section not found: " << static_cast<uint8_t>(id) << std::endl;
        return std::nullopt;
    }
}
