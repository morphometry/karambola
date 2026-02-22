cmake_minimum_required(VERSION 3.14)
project(karambola LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GSL REQUIRED)

# --- static library -----------------------------------------------------------
file(GLOB LIB_SRC      lib/*.cpp)
file(GLOB MINK_SRC     minkowski_functions/*.cpp)
file(GLOB PRINT_SRC    print_functions/*.cpp)

add_library(karambola_lib STATIC
    ${LIB_SRC}
    ${MINK_SRC}
    ${PRINT_SRC}
)

target_include_directories(karambola_lib PUBLIC
    ${PROJECT_SOURCE_DIR}/lib
    ${PROJECT_SOURCE_DIR}/minkowski_functions
    ${PROJECT_SOURCE_DIR}/print_functions
)

target_link_libraries(karambola_lib PUBLIC GSL::gsl)

# --- executable ---------------------------------------------------------------
add_executable(karambola karambola.cpp)
target_link_libraries(karambola PRIVATE karambola_lib)

# --- tests --------------------------------------------------------------------
enable_testing()

file(GLOB TEST_SRC test_suite/test_*.cpp)

add_executable(runtests test_suite/runtests.cpp ${TEST_SRC})
target_link_libraries(runtests PRIVATE karambola_lib)

add_test(
    NAME    runtests
    COMMAND runtests
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test_suite
)

# --- install ------------------------------------------------------------------
include(GNUInstallDirs)

install(TARGETS karambola     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(TARGETS karambola_lib ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY lib/         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/karambola
        FILES_MATCHING PATTERN "*.h")
