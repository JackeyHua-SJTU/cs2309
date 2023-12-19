cmake_minimum_required(VERSION 3.26)

project(proj2)

set(CMAKE_CXX_STANDARD 20)

aux_source_directory(src SOURCE)
aux_source_directory(unittest TEST)

list(REMOVE_ITEM SOURCE src/main.cpp)

include_directories(include googletest/googletest/include /opt/homebrew/Cellar/boost/1.83.0/include/
                        /opt/homebrew/Cellar/fltk/1.3.8_1/include /opt/homebrew/Cellar/libomp/17.0.6/include)
link_directories(googletest/build/lib /opt/homebrew/Cellar/boost/1.83.0/lib
                        /opt/homebrew/Cellar/fltk/1.3.8_1/lib /opt/homebrew/Cellar/libomp/17.0.6/lib)

add_executable(main ${SOURCE} src/main.cpp)
add_executable(unit_test ${TEST} src/poly.cpp)
target_link_libraries(unit_test gtest gtest_main omp)
target_link_libraries(main fltk omp)

enable_testing()
add_test(NAME unit_test COMMAND unit_test)


