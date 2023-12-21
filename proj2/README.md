# Proj 2 
## Description
This project is a variant of gallery guardian problem. We need to find the position that can cover maximum space of the gallery. An extension is that there is an obstacle inside the gallery and we have more than one guardians.

## Structure
- `src/`: source code
- `include/`: header files
- `unittest/`: unit test files
- `result/`: result figures

## Function Implemented
- one guardian case
- one guardian with obstacle case
- two guardians (time-consuming)

## Features
- Robust. Unit tested by Google test. See test files in `unittest/`.
- Fast. Use openMP to parallelize the program.
- Easy to use and control. Use CMake to manage dependencies and build the project. See `CMakeLists.txt`.
- Easy to verify. Use FLTK to visualize the result.

> Check [GitHub Link](https://github.com/JackeyHua-SJTU/cs2309/tree/main/proj2) for my commit history.


## Run
1. Install dependencies
    - CMake
    - FLTK
    - OpenMP
    - Google Test (optional)
      - `git clone https://github.com/google/googletest.git`
      - `cd googletest`
      - `mkdir build && cd build`
      - `cmake ..`
      - `make`
  
It is recommended to use `homebrew` to install dependencies on Mac.

2. Build
    - `mkdir build && cd build`
    - `cmake ..`
    - `make`
  
3. Run
    - `./main` to run the program
    - `./unit_test` to run the unit test

