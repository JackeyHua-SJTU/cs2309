#!/bin/zsh

cd src || return
g++ -c mywindow.cpp polygon.cpp picwindow.cpp -I /opt/homebrew/Cellar/fltk/1.3.8_1/include -I ../include -std=c++20
ar rcs libmain.a *.o
rm *.o
g++ main.cpp -o main -I /opt/homebrew/Cellar/fltk/1.3.8_1/include -I ../include -L /opt/homebrew/Cellar/fltk/1.3.8_1/lib/ -l fltk_gl -L /opt/homebrew/Cellar/fltk/1.3.8_1/lib/ -l fltk -L /opt/homebrew/Cellar/fltk/1.3.8_1/lib/ -l fltk_images -L /opt/homebrew/Cellar/fltk/1.3.8_1/lib/ -l fltk_forms -L . -l main -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_THREAD_SAFE -D_REENTRANT -lpthread -framework Cocoa -std=c++20
./main
