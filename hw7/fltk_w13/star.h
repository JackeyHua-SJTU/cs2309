#ifndef _STAR_H_
#define _STAR_H_

#include <FL/Fl.H>
#include <FL/Fl_Window.H>

class star : public Fl_Window {
    private:
        int number;

    public: 
        star(int num, int width, int height);
        void valid();
        void draw();
}; 

#endif