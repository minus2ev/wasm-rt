cmake_minimum_required(VERSION 3.15)

project(wasm-rt VERSION 0.1 LANGUAGES CXX)

set(HEADERS
    src/utils/leb128.h
    src/utils/types.h
    src/sections/section.h
    src/sections/type_section.h
    src/sections/func_section.h
    src/sections/mem_section.h
    src/sections/global_section.h
    src/module.h
)

set(SOURCES
    src/module.cpp
)

add_library(${PROJECT_NAME}-lib ${HEADERS} ${SOURCES})
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-lib)

add_subdirectory(test)