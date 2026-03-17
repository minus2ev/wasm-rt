#include <catch2/catch_test_macros.hpp>

#include <algorithm>

#include "../src/module.h"
#include "../src/utils/leb128.h"
#include "../src/sections/type_section.h"

TEST_CASE("Load module.wasm", "[load][sections]") {
    const std::string path = WASM_FIXTURE_PATH;
    wasm_rt::Module module;
    module.load(path);
    REQUIRE(module.is_valid());
    REQUIRE(module.version() == 1);

    using namespace wasm_rt::sections;

    // Type section
    auto section = module.section(wasm_rt::sections::SectionId::Type);
    REQUIRE(section.has_value());
    auto type_section = dynamic_cast<TypeSection*>(&section->get());
    const auto& types = type_section->types();
    const TypeSection::entry_t fn_entry{
        // int(int, int)
        Type::Func,
        {Type::I32, Type::I32},
        {Type::I32}
    };
    auto it = std::find(types.cbegin(), types.cend(), fn_entry);
    REQUIRE(it != types.end());
}

TEST_CASE("LEB128 decoding", "[leb128]") {
    std::vector<uint8_t> data = {
        0x80, 0x88, 0x04
    };
    auto it = data.cbegin();
    uint32_t v = wasm_rt::utils::decode_leb128<uint32_t>(it, data.cend());
    REQUIRE(v == 66560u);
}