project( clecta-tests )
cmake_minimum_required( VERSION 2.8 )

#option(tr064_build_tests "Build all of tr064's unit tests." OFF)
#set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGTEST_HAS_TR1_TUPLE=0" )

include_directories(
  SYSTEM             
  "${CLECTA_SOURCE_DIR}/src"
  )

file( GLOB SOURCES 
  *.cpp 
  "${CLECTA_SOURCE_DIR}/src/search.cpp"
)

configure_file( ./test_data/words50k.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY )

add_executable( ${PROJECT_NAME} ${SOURCES} )
target_link_libraries( ${PROJECT_NAME} 
                        pthread 
                        gmock 
                        gmock_main 
                        gtest 
                        gtest_main)

add_test( clecta-tests ${PROJECT_NAME} )

