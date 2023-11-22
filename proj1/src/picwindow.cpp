#include "picwindow.h"
#include <FL/Fl.H>
#include <GL/gl.h>
#include <FL/glut.H>
#include <FL/Fl_Button.H>
#include <iostream>
#include <cmath>

picwindow::picwindow(int x, int y, int w, int h, const char* l) : Fl_Gl_Window(x, y, w, h, l) {
    this->area = 0.0;
    this->scale = 0.0;
    this->click1 = false;
    this->click2 = false;
    this->button1 = new Fl_Button(30, 100, 200, 50, "click me after \nselecting plotting scale");
    this->button1->callback(button1_callback, this);
    this->button2 = new Fl_Button(30, 200, 200, 50, "click me after \nselecting edge points");
    this->button2->callback(button2_callback, this);
    this->resizable(this);
}

int picwindow::handle(int event) {
    if (event == FL_PUSH) {
        int x = Fl::event_x();  // x axis clicked by mouse
        int y = Fl::event_y();  // y axis clicked by mouse
        this->poly.points.emplace_back(x, y);  // store it into vector
        std::cout << "current size is " << this->poly.points.size() << std::endl;
        // print coordinates to console
        printf("点击坐标：(%d, %d)\n", x, y);
        this->redraw();  // 重绘窗口
    }
    return Fl_Gl_Window::handle(event);
}

void picwindow::draw() {
    // Fl_Gl_Window::draw();
    int w = this->w();
    int h = this->h();

    glColor3f(0.0f, 0.0f, 0.0f); // black
    // x-axis
    glBegin(GL_LINES);
    glVertex2f(300, h - 50);
    glVertex2f(w - 50, h - 50);
    glEnd();

    // y-axis
    glBegin(GL_LINES);
    glVertex2f(300, 50);
    glVertex2f(300, h - 50);
    glEnd();

    // x-axis arrow
    glBegin(GL_LINES);
    glVertex2f(w - 50, h - 50);
    glVertex2f(w - 50 - 10 * cos(M_PI / 6.0), h - 50 - 10 * sin(M_PI / 6.0));
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(w - 50, h - 50);
    glVertex2f(w - 50 - 10 * cos(M_PI / 6.0), h - 50 + 10 * sin(M_PI / 6.0));
    glEnd();

    // y-axis arrow
    glBegin(GL_LINES);
    glVertex2f(300, 50);
    glVertex2f(300 - 10 * sin(M_PI / 6.0), 50 + 10 * cos(M_PI / 6.0));
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(300, 50);
    glVertex2f(300 + 10 * sin(M_PI / 6.0), 50 + 10 * cos(M_PI / 6.0));
    glEnd();

    // x-axis label
    glRasterPos2f(w - 50, h - 30);
    std::string xLabel = "X Axis";
    for (auto&& c : xLabel) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    // y-axis label
    glRasterPos2f(310, 50);
    std::string yLabel = "Y Axis";
    for (auto&& c : yLabel) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    // mark on x-axis
    for (int i = 1; i <= 16; ++i) {
        int x_pos = 300 + i * 50;
        int y = h - 50;
        glPointSize(6.0f);
        glBegin(GL_POINTS);
        glVertex2f(x_pos, y);
        glEnd();

        glRasterPos2f(x_pos - 5, y + 15);
        std::string s = std::to_string(i * 50);
        for (auto&& c : s) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }

    // mark on y-axis
    for (int i = 1; i <= 13; ++i) {
        int x = 300;
        int y_pos = h - 50 - i * 50;
        glPointSize(6.0f);
        glBegin(GL_POINTS);
        glVertex2f(x, y_pos);
        glEnd();

        glRasterPos2f(x - 30, y_pos + 5);
        std::string s = std::to_string(i * 50);
        for (auto&& c : s) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }

    // this->button1->draw();
    // this->button2->draw();

    glColor3f(1.0f, 0.0f, 0.0f); // red
    // draw all points in poly.points
    for (auto&& p : this->poly.points) {
        glPointSize(6.0f);
        glBegin(GL_POINTS);
        glVertex2f(p.first, p.second);
        glEnd();
    }

    if (this->click1) {
        std::string s = "1 meter in real world equals to \n" + std::to_string(this->scale) + " pixels/units in the window";
        glRasterPos2f(30, 300);
        for (auto&& c : s) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }

    if (this->click2) {
        int size = this->poly.points.size();

        for (int i = 0; i < size; ++i) {
            glBegin(GL_LINES);
            glVertex2f(this->poly.points[i].first, this->poly.points[i].second);
            glVertex2f(this->poly.points[(i + 1) % size].first, this->poly.points[(i + 1) % size].second);
            glEnd();
        }

        glColor4f(1.0f, 0.0f, 0.0f, 0.3f); // red with alpha
        glBegin(GL_POLYGON);
        for (auto&& p : this->poly.points) {
            glVertex2f(p.first, p.second);
        }
        glEnd();

        glColor3f(1.0f, 0.0f, 0.0f); // red
        glRasterPos2f(30, 450);
        std::string s = "Area: " + std::to_string(this->area) + " square meters";
        for (auto&& c : s) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }

}

void picwindow::button1_callback(Fl_Widget *w, void *data) {
    picwindow *p = (picwindow *) data;
    p->click1 = true;
    auto p1 = p->poly.points[0];
    auto p2 = p->poly.points[1];
    p->scale = sqrt(pow(p1.first - p2.first, 2) + pow(p1.second - p2.second, 2));
    p->poly.points.clear();
    w->hide();
    p->redraw();
}

void picwindow::button2_callback(Fl_Widget *w, void *data) {
    picwindow *p = (picwindow *) data;
    p->click2 = true;
    p->poly.points.pop_back();
    p->poly.sort();
    p->area = p->poly.area();
    p->redraw();
}