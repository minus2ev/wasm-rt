#include <catch2/catch_test_macros.hpp>

#include <algorithm>

#include "../src/module.h"
#include "../src/utils/leb128.h"
#include "../src/sections/type_section.h"
#include "../src/sections/func_section.h"
#include "../src/sections/mem_section.h"
#include "../src/sections/global_section.h"
#include "../src/sections/export_section.h"
#include "../src/sections/code_section.h"

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
        Type::Func,
        {Type::I32, Type::I32},
        {Type::I32}
    };
    auto type_it = std::find(types.cbegin(), types.cend(), fn_entry);
    REQUIRE(type_it != types.cend());

    // Function section
    section = module.section(wasm_rt::sections::SectionId::Function);
    REQUIRE(section.has_value());
    auto func_section = dynamic_cast<FuncSection*>(&section->get());
    const auto& funcs = func_section->funcs();
    REQUIRE(funcs.size() >= 2);
    REQUIRE(funcs[1].type_index == 1); // the second function should have type index 1 (the one we found above)

    // Memory section
    section = module.section(wasm_rt::sections::SectionId::Memory);
    REQUIRE(section.has_value());
    auto mem_section = dynamic_cast<MemSection*>(&section->get());
    const auto& mems = mem_section->mems();
    REQUIRE(mems.size() >= 1);
    REQUIRE(mems[0].has_max == 0); // only min
    REQUIRE(mems[0].min == 2); // min size: 2

    // Global section
    section = module.section(wasm_rt::sections::SectionId::Global);
    REQUIRE(section.has_value());
    auto glob_section = dynamic_cast<GlobalSection*>(&section->get());
    const auto& globs = glob_section->globals();
    REQUIRE(globs.size() >= 1);
    REQUIRE(globs[0].type == Type::I32);
    REQUIRE(globs[0].mut == 1);            // mutable
    const std::vector<uint8_t>& expr{ 0x41, 0x80, 0x88, 0x04 };
    REQUIRE(globs[0].expr == expr);         // i32.const 66560

    // Export section
    section = module.section(wasm_rt::sections::SectionId::Export);
    REQUIRE(section.has_value());
    auto export_section = dynamic_cast<ExportSection*>(&section->get());
    const auto& exports = export_section->exports();
    REQUIRE(exports.size() > 0);
    const std::vector<uint8_t> expected_name{'a', 'd', 'd'};
    auto export_it = std::find_if(exports.cbegin(), exports.cend(), [&expected_name](const ExportSection::entry_t& entry) {
        return entry.name == expected_name;
    });
    REQUIRE(export_it != exports.end());
    REQUIRE(export_it->type == 0);      // function
    REQUIRE(export_it->index == 0x01);  // 1

    // Code section
    section = module.section(wasm_rt::sections::SectionId::Code);
    REQUIRE(section.has_value());
    auto code_section = dynamic_cast<CodeSection*>(&section->get());
    const auto& code = code_section->code();
    REQUIRE(code.size() > 0);
}

TEST_CASE("LEB128 decoding", "[leb128]") {
    std::vector<uint8_t> data = {
        0x80, 0x88, 0x04
    };
    const uint32_t expected_value = 66560u;
    auto it = data.cbegin();
    uint32_t v = wasm_rt::utils::decode_leb128<uint32_t>(it, data.cend());
    REQUIRE(v == expected_value);
}