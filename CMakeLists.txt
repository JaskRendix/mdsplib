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
    src/remark_quality_flags.c
    src/remark_thermal_pressure.c
    src/remark_precip_snow.c
    src/remark_cloud.c
    src/remark_wind_storm.c
    src/remark_visibility.c
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

    add_executable(charcmp_test
        tests/charcmp_test.c
    )
    target_include_directories(charcmp_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    target_link_libraries(charcmp_test PRIVATE metar)

    add_executable(fracpart_test
        tests/fracpart_test.c
    )
    target_include_directories(fracpart_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    target_link_libraries(fracpart_test PRIVATE metar)

    add_executable(stspack2_test
        tests/stspack2_test.c
    )
    target_include_directories(stspack2_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    target_link_libraries(stspack2_test PRIVATE metar)

    add_executable(stspack3_test
        tests/stspack3_test.c
    )
    target_include_directories(stspack3_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    target_link_libraries(stspack3_test PRIVATE metar)

    add_executable(print_decoded_metar_test
        tests/print_decoded_metar_test.c
    )
    target_include_directories(print_decoded_metar_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    target_link_libraries(print_decoded_metar_test PRIVATE metar)

    add_test(NAME metar_test COMMAND metar_test)
    add_test(NAME antoi_test COMMAND antoi_test)
    add_test(NAME charcmp_test COMMAND charcmp_test)
    add_test(NAME fracpart_test COMMAND fracpart_test)
    add_test(NAME stspack2_test COMMAND stspack2_test)
    add_test(NAME stspack3_test COMMAND stspack3_test)
    add_test(NAME print_decoded_metar_test COMMAND print_decoded_metar_test)

endif()
