#pragma once
#include <GL/glut.h>

class Window
{
private:
    int _width;
    int _height;
    int _x_pos;
    int _y_pos;

public:
    int descriptor;
    Window(int width, int height);
    Window(int width, int height, int x_pos, int y_pos);

    auto Init(int argc, char **argv) -> void;
    auto Draw(void (*callback)()) -> void;
    auto RegisterKeyboardCb(void (*callback)(unsigned char key, int x, int y)) -> void;
    auto RegisterSpecialKeyboardDownCb(void (*callback)(int key, int x, int y)) -> void;
    auto RegisterSpecialKeyboardUpCb(void (*callback)(int key, int x, int y)) -> void;
    auto RegisterKeyboardUpCb(void (*callback)(unsigned char key, int x, int y)) -> void;
};

Window::Window(int width, int height) : _width(width), _height(height), _x_pos(0), _y_pos(0) {}

auto Window::Init(int argc, char **argv) -> void
{
    glutInit(&argc, argv);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(_width, _height);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    descriptor = glutCreateWindow("Platformer");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

auto Window::Draw(void (*callback)()) -> void
{
    glutDisplayFunc(callback);
    glutMainLoop();
}

auto Window::RegisterKeyboardCb(void (*callback)(unsigned char key, int x, int y)) -> void
{
    glutKeyboardFunc(callback);
}

auto Window::RegisterKeyboardUpCb(void (*callback)(unsigned char key, int x, int y)) -> void
{
    glutKeyboardUpFunc(callback);
}

auto Window::RegisterSpecialKeyboardDownCb(void (*callback)(int key, int x, int y)) -> void
{
    glutSpecialFunc(callback);
}


auto Window::RegisterSpecialKeyboardUpCb(void (*callback)(int key, int x, int y)) -> void
{
    glutSpecialUpFunc(callback);
}