cmake_minimum_required(VERSION 3.15)

project(wasm-rt VERSION 0.1 LANGUAGES CXX)

set(SOURCES
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

add_subdirectory(test)