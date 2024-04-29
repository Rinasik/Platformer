#include <GL/glut.h>
#include <math.h>
#include <fstream>

#include "sources/window/window.hpp"
#include "sources/map/map.hpp"
#include "sources/utils/constants.hpp"
#include "sources/entities/enemy/enemy.hpp"
#include "sources/entities/hero/hero.hpp"
#include "sources/entities/platform/platform.hpp"
#include "sources/machine/machine.hpp"
#include "sources/engine/engine.hpp"

auto engine = Engine("./resources");

Hero hero;
std::vector<Entity *> entities;

void drawCb()
{
    // Drawing map
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    engine.Draw(hero, entities);

    glutSwapBuffers();
}

void EngineCb(int time)
{
    engine.UpdateState(hero, entities);

    glutPostRedisplay();
    glutTimerFunc(1000 / FRAME_FREQUENCY, EngineCb, 0);
}

void specialKeyDown(int key, int x, int y)
{
    hero.HandleClickDown(key);
    glutPostRedisplay();
}

void specialKeyUp(int key, int x, int y)
{
    hero.HandleClickUp(key);
    glutPostRedisplay();
}

void handleKey(unsigned char key, int x, int y)
{
    if (key == 'q')
    {
        engine.InitState(hero, entities);
    }
}

int main(int argc, char **argv)
{
    engine.InitState(hero, entities);

    auto window = Window(SCREEN_WIDTH, SCREEN_HEIGHT);
    window.Init(argc, argv);

    glutTimerFunc(1000 / FRAME_FREQUENCY, EngineCb, 0);

    window.RegisterSpecialKeyboardDownCb(specialKeyDown);
    window.RegisterSpecialKeyboardUpCb(specialKeyUp);

    window.RegisterKeyboardCb(handleKey);

    window.Draw(drawCb);

    return 0;
}