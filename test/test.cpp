#include <catch2/catch_test_macros.hpp>

#include "../src/module.h"
#include "../src/leb128.h"

TEST_CASE("Load module.wasm", "[runtime][fixture]") {
    const std::string path = WASM_FIXTURE_PATH;
    wasm_rt::Module module;
    module.load(path);
    REQUIRE(module.is_valid());
    REQUIRE(module.version() == 1);
}

TEST_CASE("LEB128 decoding", "[leb128]") {
    std::vector<uint8_t> data = {
        0x80, 0x88, 0x04
    };
    auto it = data.cbegin();
    uint32_t v = wasm_rt::utils::decode_leb128<uint32_t>(it, data.cend());
    REQUIRE(v == 66560u);
}