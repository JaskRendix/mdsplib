cmake_minimum_required(VERSION 3.12...3.25)
project(mdsplib LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif()

add_library(metar STATIC
    src/antoi.c
    src/charcmp.c
    src/decode_metar.c
    src/decode_metar_remark.c
    src/fracpart.c
    src/print_decoded_metar.c
    src/stspack2.c
    src/stspack3.c
)

target_include_directories(metar
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(MSVC)
    target_compile_options(metar PRIVATE
        /wd4710
        /wd4711
        /wd4068
        /wd4109
    )
endif()

include(CTest)

if(BUILD_TESTING)

    add_executable(metar_test
        tests/metar_test.c
    )
    target_include_directories(metar_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    target_link_libraries(metar_test PRIVATE metar)

    add_executable(antoi_test
        tests/antoi_test.c
    )
    target_include_directories(antoi_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    target_link_libraries(antoi_test PRIVATE metar)

    add_test(NAME metar_test COMMAND metar_test)
    add_test(NAME antoi_test COMMAND antoi_test)

endif()
