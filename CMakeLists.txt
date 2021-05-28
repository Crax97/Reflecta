cmake_minimum_required(VERSION 3.13)
project(reflecta)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

include_directories(include)

set(SOURCES)

set(REFELCTA_SOURCES 
    src/object.cpp 
    src/meta_descriptor.cpp
    src/box_objects.cpp
)

set(TEST_SOURCES ${REFELCTA_SOURCES} tests/main.cpp tests/tests.cpp)

set(CMAKE_CXX_STANDARD 17)

add_library(reflecta ${REFELCTA_SOURCES})
add_executable(test ${TEST_SOURCES})
target_compile_features(test PRIVATE cxx_std_17)
