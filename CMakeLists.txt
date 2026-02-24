cmake_minimum_required(VERSION 3.15)

project(wasm-rt VERSION 0.1 LANGUAGES CXX)

set(HEADERS
    src/module.h
)

set(SOURCES
    src/module.cpp
)

add_library(${PROJECT_NAME}-lib ${HEADERS} ${SOURCES})
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-lib)

add_subdirectory(test)