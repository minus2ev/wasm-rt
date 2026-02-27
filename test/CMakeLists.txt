cmake_minimum_required(VERSION 3.15)

Include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.12.0
)
FetchContent_MakeAvailable(Catch2)

add_executable(wasm-rt-test test.cpp)
target_link_libraries(wasm-rt-test PRIVATE Catch2::Catch2WithMain wasm-rt-lib)

# ========= wasm module ==========

include(ExternalProject)

ExternalProject_Add(wasm_module
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/module"
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/module_build"
    INSTALL_COMMAND ""
)

add_dependencies(wasm-rt-test wasm_module)

target_compile_definitions(wasm-rt-test PRIVATE 
    WASM_FIXTURE_PATH="${CMAKE_CURRENT_BINARY_DIR}/module_build/module.wasm"
)