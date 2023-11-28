#!/bin/zsh

g++ main.cpp -o main -I /opt/homebrew/Cellar/boost/1.83.0/include -I /opt/homebrew/Cellar/fltk/1.3.8_1/include -L /opt/homebrew/Cellar/fltk/1.3.8_1/lib/ -l fltk -L /opt/homebrew/Cellar/fltk/1.3.8_1/lib/ -l fltk_images -L /opt/homebrew/Cellar/boost/1.83.0/lib -l boost_random -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_THREAD_SAFE -D_REENTRANT -lpthread -framework Cocoa -std=c++20