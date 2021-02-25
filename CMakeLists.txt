cmake_minimum_required(VERSION 3.14.0)

include(FetchContent)

set(CMAKE_CXX_STANDARD 17)

project(ha-dsp-tool-box)

add_subdirectory(external)

add_library(dsp-tool-box STATIC
    include/ha/dsp_tool_box/modulation/modulation_phase.h
    include/ha/dsp_tool_box/modulation/modulation_phase.inl.h
    source/modulation/modulation_phase.cpp
)

target_include_directories(dsp-tool-box
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/source
)

enable_testing()

add_executable(dsp-tool-box_test
    test/modulation_test.cpp
)

target_link_libraries(dsp-tool-box_test
    PRIVATE
        dsp-tool-box
        gtest
        gtest_main
)