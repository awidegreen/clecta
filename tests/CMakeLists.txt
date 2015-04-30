include_directories(
  SYSTEM             
  "../src"
  )

file( GLOB SOURCES 
  *.cpp 
  "../src/search.cpp"
  "../src/matcher.cpp"
)

configure_file( ./test_data/words50k.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY )

add_executable( clecta-tests ${SOURCES} )

target_link_libraries( clecta-tests
                        pthread 
                        gtest 
                        gtest_main)

add_test( clecta-tests clecta-tests)

