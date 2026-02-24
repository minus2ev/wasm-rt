#include <catch2/catch_test_macros.hpp>

#include "../src/module.h"

TEST_CASE("Load module.wasm", "[runtime][fixture]") {
    const std::string path = WASM_FIXTURE_PATH;
    wasm_rt::Module module;
    module.load(path);
    REQUIRE(module.is_valid());
    REQUIRE(module.version() == 1);
}