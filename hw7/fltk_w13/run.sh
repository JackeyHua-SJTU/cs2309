#!/bin/zsh

# Library
g++ -c star.cpp binary_tree.cpp ../revised-gui/*.cpp -I /opt/homebrew/Cellar/fltk/1.3.8_1/include -I ../revised-gui -std=c++20
ar rcs libmy.a *.o
rm *.o

g++ main.cpp -o main -I /opt/homebrew/Cellar/fltk/1.3.8_1/include -I . -I ../revised-gui -L /opt/homebrew/Cellar/fltk/1.3.8_1/lib/ -l fltk -L /opt/homebrew/Cellar/fltk/1.3.8_1/lib/ -l fltk_images -L . -l my -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_THREAD_SAFE -D_REENTRANT -lpthread -framework Cocoa -std=c++20
rm libmy.a