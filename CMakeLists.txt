cmake_minimum_required(VERSION 3.15.0)

include(FetchContent)

project(ha-dsp-tool-box)

add_subdirectory(external)

add_library(dsp-tool-box STATIC
    include/ha/dsp_tool_box/core/types.h
    include/ha/dsp_tool_box/filtering/one_pole.h
    include/ha/dsp_tool_box/modulation/modulation_phase.h
    source/filtering/one_pole.cpp
    source/modulation/modulation_phase.cpp
)

target_include_directories(dsp-tool-box
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/source
)

target_compile_features(dsp-tool-box
    PUBLIC 
        cxx_std_17
)

enable_testing()

add_executable(dsp-tool-box_test
    test/modulation_test.cpp
    test/one_pole_test.cpp
)

target_link_libraries(dsp-tool-box_test
    PRIVATE
        dsp-tool-box
        gtest
        gtest_main
)

add_test(NAME dsp-tool-box_test 
    COMMAND 
        dsp-tool-box_test
)
