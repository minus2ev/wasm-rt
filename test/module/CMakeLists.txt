cmake_minimum_required(VERSION 3.15)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

project(wasm_module)

add_library(module_obj OBJECT module.cpp)

target_compile_options(module_obj PRIVATE 
    --target=wasm32 
    -O3
)

# Custom link command to prevent cmake treating the project
# as Linux target one and adding unsupported linker flags
add_custom_command(
    OUTPUT module.wasm
    COMMAND clang++ --target=wasm32 -nostdlib -Wl,--no-entry -Wl,--export-all 
            $<TARGET_OBJECTS:module_obj> -o module.wasm
    DEPENDS module_obj
)

add_custom_target(wasm_module ALL DEPENDS module.wasm)