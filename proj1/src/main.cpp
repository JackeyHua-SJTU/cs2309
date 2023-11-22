#include "mywindow.h"
#include <FL/Fl_PNG_Image.H>


extern void button_callback_1(Fl_Widget* w, void* data);

extern void button_callback_2(Fl_Widget* w, void* data);

int main() {
    // mywindow window(800, 600, "main");
    // Fl_Image_Surface imgSurface(window.w() - 200, window.h() - 200);
    Fl_PNG_Image image("../3.png");
    int w = image.w();
    int h = image.h();
    mywindow window(3 * w, 3 * h, "main");
    // image.draw(100, 100, 400, 300);
    // imgSurface.draw(image, 100, 100);
    Fl_Box box(400, 150, w, h);
    box.image(&image);  // 复制图片以防止被释放

    Fl_Button button1(30, 100, 200, 50, "click me after \nselecting plotting scale");
    button1.callback(button_callback_1, &window);
    Fl_Button button2(30, 200, 200, 50, "click me after \nselecting edge points");
    button2.callback(button_callback_2, &window);
//    // 设置控件的大小
//    box.resize(100, 50, 200, 150);
//
    // window.add(&imgSurface);
    
    window.show();
    return Fl::run();
}