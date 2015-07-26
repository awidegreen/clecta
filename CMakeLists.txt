cmake_minimum_required (VERSION 2.8.7)
project(clecta)

set(CLECTA_VERSION "0.8")

add_definitions(
    -std=c++11 
    -Wall 
    -pedantic 
    -Wextra
)

file(GLOB SOURCES
  "src/*.cpp"
  )

configure_file(
  "${CMAKE_SOURCE_DIR}/cmake/config.hh.in" 
  "${CMAKE_CURRENT_BINARY_DIR}/config.hh"
  @ONLY
  )
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ncursesw pthread)

find_package(GTest REQUIRED)
if (GTEST_FOUND)
  message(STATUS "Found Gtest, including unit tests.")
  enable_testing()
  add_subdirectory(tests) 
else()
  message(STATUS "Found NOT Gtest, unit tests not included in build.")
endif()

