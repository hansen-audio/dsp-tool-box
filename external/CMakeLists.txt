cmake_minimum_required(VERSION 3.15.0)

include(FetchContent)

if(NOT HA_GIT_REPOSITORY_URL)
    set(HA_GIT_REPOSITORY_URL "https://www.github.com")
endif()

add_subdirectory(googletest)
