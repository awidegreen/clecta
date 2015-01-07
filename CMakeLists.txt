cmake_minimum_required (VERSION 2.8)
project(CLECTA)

add_definitions(
    -std=c++11 
    # Other flags
)

file(GLOB SOURCES
  "src/*.cpp"
  )

add_executable(clecta ${SOURCES})
target_link_libraries(clecta ncursesw)

#add_subdirectory(src) 

find_package(GTest REQUIRED)
if (GTEST_FOUND)
  message(STATUS "Found Gtest, including unit tests.")
  enable_testing()
  add_subdirectory(tests) 
  #add_test( AllTr064Tests tr064_tests  )
else()
  message(STATUS "Found NOT Gtest, unit tests not included in build.")
endif()

