#!/bin/bash

cd src || return
g++ -c mywindow.cpp polygon.cpp picwindow.cpp -I ../include -std=c++20
ar rcs libmain.a *.o
rm *.o
g++ main.cpp -o main -I ../include -l fltk -l fltk_gl -l fltk_images -L . -l main -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_THREAD_SAFE -D_REENTRANT -lpthread -framework Cocoa -std=c++20
./main
