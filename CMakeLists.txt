cmake_minimum_required(VERSION 3.15)

project(warun VERSION 0.1 LANGUAGES CXX)

set(SOURCES
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})