cmake_minimum_required(VERSION 3.26)
project(polygon)
# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(ROOT_DIR ${CMAKE_SOURCE_DIR})

# enable_testing()

# header file directory, explicit tell the path of header files
# 头文件路径，用于src文件夹下头文件路径的解析
include_directories(include ${ROOT_DIR}/googletest/googletest/include
                    /opt/homebrew/Cellar/fltk/1.3.8_1/include)

# Add directories in which the linker will look for libraries.
# 连接阶段所需的文件所在的路径
# link_directories(${ROOT_DIR}/googletest/build/lib)
link_directories(/opt/homebrew/Cellar/fltk/1.3.8_1/lib)

# assign DIR_SRCS (a variable) to all .cpp and .c (source) files in src directory
# will not recursively find sub-directories
aux_source_directory(src DIR_SRCS)
aux_source_directory(src/tests DIR_TESTS)

# All source files that needs to be compiled
# only 1 main function is allowed
add_executable(polygon ${DIR_SRCS})

# Link libraries
# 将libgtest.a libgtest_main.a链接到gtest
target_link_libraries(polygon libfltk.a libfltk_images.a)

# add_test(unitTest polygon)