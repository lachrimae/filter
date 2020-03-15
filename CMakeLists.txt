cmake_minimum_required(VERSION 3.10)

project(filter VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(config.h.in config.h)

add_subdirectory(src)
list(APPEND EXTRA_LIBS dsp)
list(APPEND EXTRA_LIBS AudioFile)
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/src")

add_executable(filter main.cpp)

target_link_libraries(filter PUBLIC ${EXTRA_LIBS})

target_include_directories(filter PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           ${EXTRA_INCLUDES}
                           )
