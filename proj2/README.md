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
- two guardians
  - prune the search space to randomly select 40 points

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

```bash
git clone https://github.com/google/googletest.git
cd googletest
mkdir build && cd build
cmake ..
make
```
  
It is recommended to use `homebrew` to install dependencies on Mac.

2. Build
    - `mkdir build && cd build`
    - `cmake ..`
    - `make`
  
3. Run
    - `./main` to run the program
    - `./unit_test` to run the unit test

## Test case
1. points: `vector<pair<double, double>> p1 = {{100.0, 100.0}, {500.0, 100.0}, {400.0, 220.0}, {600.0, 310.0}, {600.0, 200.0}, {700.0, 300.0}, {800.0, 500.0}, {490.0, 500.0}, {700.0, 700.0}, {400.0, 600.0}, {300.0, 300.0}};`
    area: 159137.484574
    pos : 318 318
    visualized result: `result/result1.png` and `result/test1.png`
2. points:`vector<pair<double, double>> points = {{100.0, 100.0}, {150.0, 300.0}, {170.0, 130.0}, {300.0, 130.0}, {320.0, 300.0}, {350.0, 130.0}, {450.0, 130.0}, {470.0, 300.0}, {500.0, 100.0}};`
    area: 17526.274591
    pos : 173 100
    visualized result: `result/result2.png` and `result/test2.png`
3. points: `vector<pair<double, double>> p1 = {{100.0, 100.0}, {500.0, 100.0}, {400.0, 220.0}, {600.0, 310.0}, {600.0, 200.0}, {700.0, 300.0}, {800.0, 500.0}, {490.0, 500.0}, {700.0, 700.0}, {400.0, 600.0}, {300.0, 300.0}};`
   obstacle: `vector<pair<double, double>> o1 = {{400, 350}, {400, 450}, {600, 450}, {600, 350}};`
    area: 93427.4107
    pos : 301 301
    visualized result: `result/result3.png` and `result/test3.png`

4. points: `vector<pair<double, double>> points = {{100.0, 100.0}, {150.0, 300.0}, {170.0, 130.0}, {300.0, 130.0}, {320.0, 300.0}, {350.0, 130.0}, {450.0, 130.0}, {470.0, 300.0}, {500.0, 100.0}};`
   two cameras
   area: 21694.597990
   pos: 355 100
   pos: 170 114
   visualized result: `result/result4.png` and `result/test4.png`