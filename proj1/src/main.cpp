#include "mywindow.h"
#include "windemo.h"
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_Input.H>
#include <iostream>
#include <string>
#include <vector>

extern void button_callback_1(Fl_Widget* w, void* data);

extern void button_callback_2(Fl_Widget* w, void* data);

extern void input_callback(Fl_Widget* w, void* data);

int main() {
    // -----------------------------GUI Input version---------------------------------
    // // 输入绝对路径
//     std::string path;
//     std::cout << "please input the ABSOLUTE path of the image" << std::endl;
//     std::cin >> path;

//     bool flag; // false for png, true for jpeg
//     int pos = path.find('.');
//     if (path.substr(pos + 1) == "png") {
//         flag = false;
//     } else if (path.substr(pos + 1) == "jpeg") {
//         flag = true;
//     } else {
//         std::cout << "invalid image format" << std::endl;
//         return 0;
//     }
//     Fl_PNG_Image* png;
//     Fl_JPEG_Image* jpeg;
//     int w, h;

//     if (flag) {
//         jpeg = new Fl_JPEG_Image(path.c_str());
//         w = jpeg->w();
//         h = jpeg->h();
//         if (w == 0 || h == 0) {
//             std::cout << "can not load the image, check the path." << std::endl;
//             return 1;
//         }
//     } else {
//         png = new Fl_PNG_Image(path.c_str());
//         w = png->w();
//         h = png->h();
//         if (w == 0 || h == 0) {
//             std::cout << "can not load the image, check the path." << std::endl;
//             return 1;
//         }
//     }

//     double base;
//     std::cout << "Please input the base of your scale, E.g. 16 pixel / 200m, then input 200" << std::endl;
//     std::cin >> base;
//     mywindow window(w + 400, h + 100, base, "main");
//     // std::cout << "w = " << w << " h = " << h << std::endl;
//     std::cout << "type 0 if you want to assign scale via typing in GUI, type 1 if you want to assign scale via clicking in GUI" << std::endl;
//     int type;
//     std::cin >> type;

//     Fl_Box box(340, 25, w, h);
//     if (flag) {
//         box.image(jpeg);
//     } else {
//         box.image(png);
//     }
//     Fl_Input input(50, 70, 200, 30, "Input:");
//     Fl_Button button(110, 100, 60, 30, "Submit");
//     callbackPack pack(&input, &window);
//     button.callback(input_callback, &pack);

//     Fl_Button button1(30, h / 5, 200, 50, "click me after \nselecting plotting scale");
//     button1.callback(button_callback_1, &window);

//     if (!type) {
//         button1.hide();
//     } else {
//         window.finish_input();
//         input.hide();
//         button.hide();
//     }
    
//     Fl_Button button2(30, h * 2 / 5, 200, 50, "click me after \nselecting edge points");
//     button2.callback(button_callback_2, &window);
// //    // 设置控件的大小
// //     box.resize(100, 50, 200, 150);
//     window.show();


// -----------------------------Console input version---------------------------------
    std::string path;
    std::cout << "please input the ABSOLUTE path of the image" << std::endl;
    std::cin >> path;

    bool flag; // false for png, true for jpeg
    int pos = path.find('.');
    if (path.substr(pos + 1) == "png") {
        flag = false;
    } else if (path.substr(pos + 1) == "jpeg") {
        flag = true;
    } else {
        std::cout << "invalid image format" << std::endl;
        return 0;
    }
    Fl_PNG_Image* png;
    Fl_JPEG_Image* jpeg;
    int w, h;

    if (flag) {
        jpeg = new Fl_JPEG_Image(path.c_str());
        w = jpeg->w();
        h = jpeg->h();
        if (w == 0 || h == 0) {
            std::cout << "can not load the image, check the path." << std::endl;
            return 1;
        }
    } else {
        png = new Fl_PNG_Image(path.c_str());
        w = png->w();
        h = png->h();
        if (w == 0 || h == 0) {
            std::cout << "can not load the image, check the path." << std::endl;
            return 1;
        }
    }


    std::cout << "please input the scale of the image, e.g if scale is 1 pixel / meter, then input 1" << std::endl;
    double scale;
    std::cin >> scale;

    std::cout << "please input points. End with EOF/Ctrl + D" << std::endl;
    std::vector<std::pair<double, double>> points;
    // 默认输入点的坐标是相对于图片的左上角，而GUI对图片进行了一定的移位，所以需要一定的修改
    double x, y;
    while (std::cin >> x >> y) {
        points.emplace_back(x + 340, y + 25);
    }
    windemo win(w + 400, h + 100, std::move(points), scale, "windemo");
    Fl_Box* box1 = new Fl_Box(340, 25, w, h);
    if (flag) {
        box1->image(jpeg);
    } else {
        box1->image(png);
    }

    win.show();
    return Fl::run();
}