#include "mywindow.h"
#include <FL/Fl_PNG_Image.H>

//class MyWindow : public Fl_Window {
//public:
//    MyWindow(int width, int height, const char* title) : Fl_Window(width, height, title) {}
//
//    void draw() {
//        Fl_Window::draw();
//
//        // 设置坐标轴的起点和终点
//        int xStart = 50;
//        int yStart = 50;
//        int xEnd = w() - 50;
//        int yEnd = h() - 50;
//
//        // 绘制 x 轴
//        fl_color(FL_BLACK);
//        fl_line(xStart, yStart, xEnd, yStart);
//
//        // 绘制 y 轴
//        fl_line(xStart, yStart, xStart, yEnd);
//
//        // 绘制 x 轴箭头
//        fl_line(xEnd - 10, yStart - 5, xEnd, yStart);
//        fl_line(xEnd - 10, yStart + 5, xEnd, yStart);
//
//        // 绘制 y 轴箭头
//        fl_line(xStart - 5, yEnd - 10, xStart, yEnd);
//        fl_line(xStart + 5, yEnd - 10, xStart, yEnd);
//
//        // 绘制 x 轴刻度
//        int xTickStart = xStart + 50;
//        int xTickEnd = xEnd - 50;
//        int xTickY = yStart - 10;
//        int xTickTextY = yStart - 20;
//        for (int x = xTickStart; x <= xTickEnd; x += 50) {
//            fl_line(x, yStart, x, xTickY);
//            fl_draw(std::to_string(x - xStart).c_str(), x - 10, xTickTextY);
//        }
//        fl_draw("x axis", xEnd + 10, xTickTextY);
//
//        // 绘制 y 轴刻度
//        int yTickStart = yStart + 50;
//        int yTickEnd = yEnd - 50;
//        int yTickX = xStart - 10;
//        int yTickTextX = xStart - 40;
//        for (int y = yTickStart; y <= yTickEnd; y += 50) {
//            fl_line(xStart, y, yTickX, y);
//            fl_draw(std::to_string(y - yStart).c_str(), yTickTextX, y + 5);
//        }
//        fl_draw("y axis", yTickTextX, yEnd + 10);
//    }
//};

//void button_callback_1(Fl_Widget* w, void* data) {
//    auto win = (mywindow*)data;
//    points.clear();
//    w->hide();
//    win->redraw();
//}
//
//void button_callback_2(Fl_Widget* w, void* data) {
//    auto win = (mywindow*)data;
//    points.pop_back();
//    win->initPoly();
//}

// TODO: 想办法将click1和click2封装到mywindow类中；完善GUI页面的排版；完善报错机制

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