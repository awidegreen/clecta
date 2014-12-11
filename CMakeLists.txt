cmake_minimum_required (VERSION 3.0)
project(CLECTA)

ADD_DEFINITIONS(
    -std=c++11 
    # Other flags
)

file(GLOB SOURCES
  "src/*.cpp"
  #"src/*.hh"
  )

add_executable(clecta ${SOURCES})
target_link_libraries(clecta ncursesw)

