#include <catch2/catch_test_macros.hpp>

#include <algorithm>

#include "../src/module.h"
#include "../src/utils/leb128.h"
#include "../src/sections/type_section.h"
#include "../src/sections/func_section.h"
#include "../src/sections/mem_section.h"

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

    // Functions section
    section = module.section(wasm_rt::sections::SectionId::Function);
    REQUIRE(section.has_value());
    auto func_section = dynamic_cast<FuncSection*>(&section->get());
    const auto& funcs = func_section->funcs();
    REQUIRE(funcs.size() >= 2);
    REQUIRE(funcs[1] == 1); // the second function should have type index 1 (the one we found above)

    // Memory section
    section = module.section(wasm_rt::sections::SectionId::Memory);
    REQUIRE(section.has_value());
    auto mem_section = dynamic_cast<MemSection*>(&section->get());
    const auto& mems = mem_section->mems();
    REQUIRE(mems.size() >= 1);
    REQUIRE(std::get<0>(mems[0]) == 0); // only min
    REQUIRE(std::get<1>(mems[0]) == 2); // min size: 2
}

TEST_CASE("LEB128 decoding", "[leb128]") {
    std::vector<uint8_t> data = {
        0x80, 0x88, 0x04
    };
    auto it = data.cbegin();
    uint32_t v = wasm_rt::utils::decode_leb128<uint32_t>(it, data.cend());
    REQUIRE(v == 66560u);
}