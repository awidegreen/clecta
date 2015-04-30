cmake_minimum_required (VERSION 2.8)
project(clecta)

add_definitions(
    -std=c++11 
    # Other flags
)

file(GLOB SOURCES
  "src/*.cpp"
  )

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ncursesw)

find_package(GTest REQUIRED)
if (GTEST_FOUND)
  message(STATUS "Found Gtest, including unit tests.")
  enable_testing()
  add_subdirectory(tests) 
  #add_test( AllClectaTests clecta-tests  )
else()
  message(STATUS "Found NOT Gtest, unit tests not included in build.")
endif()

