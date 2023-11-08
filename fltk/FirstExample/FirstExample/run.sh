#!/bin/zsh

g++ -c Graph.cpp GUI.cpp Simple_window.cpp Window.cpp -I . -I /opt/homebrew/Cellar/fltk/1.3.8_1/include
ar rc libfirstexample.a Graph.o GUI.o Simple_window.o Window.o
rm *.o
g++ FirstExample.cpp -o firstexample -I /opt/homebrew/Cellar/fltk/1.3.8_1/include -I . -L /opt/homebrew/Cellar/fltk/1.3.8_1/lib/ -l fltk -L /opt/homebrew/Cellar/fltk/1.3.8_1/lib/ -l fltk_images -L . -l firstexample -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_THREAD_SAFE -D_REENTRANT -lpthread -framework Cocoa