cmake_minimum_required(VERSION 3.26)

project(proj2)

set(CMAKE_CXX_STANDARD 20)

aux_source_directory(src SOURCE)
aux_source_directory(unittest TEST)

list(REMOVE_ITEM SOURCE src/main.cpp)

include_directories(include googletest/googletest/include /opt/homebrew/Cellar/boost/1.83.0/include/)
link_directories(googletest/build/lib /opt/homebrew/Cellar/boost/1.83.0/lib)

add_executable(main ${SOURCE} src/main.cpp)
add_executable(unit_test ${TEST} ${SOURCE})
target_link_libraries(unit_test gtest gtest_main)

enable_testing()
add_test(NAME unit_test COMMAND unit_test)


