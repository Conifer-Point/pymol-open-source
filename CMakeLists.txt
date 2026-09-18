cmake_minimum_required(VERSION 3.13)

project(${TARGET_NAME})

set(CMAKE_VERBOSE_MAKEFILE on)

add_library(${TARGET_NAME} SHARED ${ALL_SRC})

target_compile_options(${TARGET_NAME} PRIVATE ${ALL_COMP_ARGS})

set_target_properties(${TARGET_NAME} PROPERTIES SUFFIX ${SHARED_SUFFIX})

target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)

set_target_properties(${TARGET_NAME} PROPERTIES PREFIX "")

target_include_directories(${TARGET_NAME} PUBLIC ${ALL_INC_DIR})

target_link_directories(${TARGET_NAME} PUBLIC ${ALL_LIB_DIR})


if(APPLE)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -undefined dynamic_lookup")
endif()

target_link_libraries(${TARGET_NAME}
    ${ALL_LIB}
    ${ALL_EXT_LINK}
)

target_compile_definitions(${TARGET_NAME} PUBLIC ${ALL_DEF})

if(EMSCRIPTEN)
    # Pyodide side modules must be real shared objects. Under plain `emcmake`
    # SHARED silently degrades to STATIC; pyodide-build's toolchain file
    # enables shared libs. Make the decision visible in the build log.
    get_property(_pymol_shared GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS)
    get_target_property(_pymol_type ${TARGET_NAME} TYPE)
    message(STATUS "emscripten: TARGET_SUPPORTS_SHARED_LIBS=${_pymol_shared} "
                   "target TYPE=${_pymol_type} toolchain=${CMAKE_TOOLCHAIN_FILE}")
    if(NOT _pymol_type STREQUAL "SHARED_LIBRARY")
        message(FATAL_ERROR "${TARGET_NAME} would not be a shared library (use pyodide build)")
    endif()
endif()
