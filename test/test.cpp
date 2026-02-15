#include <catch2/catch_test_macros.hpp>

#include <iostream>

TEST_CASE("Load module.wasm", "[runtime][fixture]") {
    std::string path = WASM_FIXTURE_PATH;

    std::cout << "module.wasm path: " << path << std::endl;
}