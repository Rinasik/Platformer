#include <GL/glut.h>
#include <math.h>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <unordered_set>

#include "sources/window/window.hpp"
#include "sources/map/map.hpp"
#include "sources/utils/constants.hpp"
#include "sources/objects/hero/hero.hpp"
#include "sources/objects/platform/platform.hpp"
#include "sources/machine/machine.hpp"
#include "sources/engine/engine.hpp"

Engine engine;

std::shared_ptr<Hero> hero;
std::shared_ptr<Boss> boss;
std::unordered_set<std::shared_ptr<Entity>> entities;

void drawCb()
{
    // Drawing map
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    engine.Draw(hero, boss, entities);

    glutSwapBuffers();
}

void EngineCb(int time)
{
    engine.UpdateState(hero, boss, entities);

    glutPostRedisplay();
    glutTimerFunc(1000 / FRAME_FREQUENCY, EngineCb, 0);
}

void specialKeyDown(int key, int x, int y)
{
    hero->HandleSpecialClickDown(key);
    glutPostRedisplay();
}

void specialKeyUp(int key, int x, int y)
{
    hero->HandleSpecialClickUp(key);
    glutPostRedisplay();
}

void handleKey(unsigned char key, int x, int y)
{
    hero->HandleClickDown(key, entities);

    if (key == 'q')
    {
        engine.InitState(hero, boss, entities);
    }
}

void initTextures()
{
    WALL = new Texture("images/Wall.png");
    MAGMA = new Texture("images/Magma.png");
    BACKGROUND = new Texture("images/Background.png", true);
    FIREBALL = new Texture("images/Fireball.png", true);
    ARROW = new Texture("images/Arrow.png", true);
    BOX = new Texture("images/Box.png", true);
    CHEST = new Texture("images/Chest.png", true);
    HEART = new Texture("images/Heart.png", true);
    MAXLIFE = new Texture("images/MaxLife.png", true);
    THREEHEART = new Texture("images/3Heart.png", true);
    KEY = new Texture("images/Key.png", true);
    BOW = new Texture("images/Bow.png", true);
    QUIVER = new Texture("images/Quiver.png", true);
    DOUBLEJUMP = new Texture("images/DoubleJump.png", true);
}

int main(int argc, char **argv)
{
    std::srand(std::time(nullptr));

    auto window = Window(SCREEN_WIDTH, SCREEN_HEIGHT);
    window.Init(argc, argv);
    initTextures();

    engine = Engine("./resources");
    engine.InitState(hero, boss, entities);

    glutTimerFunc(1000 / FRAME_FREQUENCY, EngineCb, 0);

    window.RegisterSpecialKeyboardDownCb(specialKeyDown);
    window.RegisterSpecialKeyboardUpCb(specialKeyUp);

    window.RegisterKeyboardCb(handleKey);

    window.Draw(drawCb);

    return 0;
}