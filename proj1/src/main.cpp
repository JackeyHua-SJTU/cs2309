#include "mywindow.h"
#include <FL/Fl_PNG_Image.H>
#include <iostream>


extern void button_callback_1(Fl_Widget* w, void* data);

extern void button_callback_2(Fl_Widget* w, void* data);

// TODO: 完成两个版本的main函数，一个完全由图形页面进行输入，还有一个是通过命令行输入数据

int main() {
    // 输入绝对路径
    Fl_PNG_Image image("/Users/huazhendong/Desktop/sjtu/大三上/problem solving/proj1/2.png");
    int w = image.w();
    int h = image.h();
    mywindow window(w + 400, h + 100, "main");
    std::cout << "w = " << w << " h = " << h << std::endl;
    // image.draw(100, 100, 400, 300);
    // imgSurface.draw(image, 100, 100);
    Fl_Box box(340, 25, w, h);
    box.image(&image);  // 复制图片以防止被释放

    Fl_Button button1(30, h / 5, 200, 50, "click me after \nselecting plotting scale");
    button1.callback(button_callback_1, &window);
    Fl_Button button2(30, h * 2 / 5, 200, 50, "click me after \nselecting edge points");
    button2.callback(button_callback_2, &window);
//    // 设置控件的大小
//    box.resize(100, 50, 200, 150);
//
    // window.add(&imgSurface);
    
    window.show();
    return Fl::run();
}