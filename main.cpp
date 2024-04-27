#include <GL/glut.h>
#include <math.h>
#include <fstream>

#include "window.hpp"
#include "map.hpp"
#include "constants.hpp"
#include "enemy.hpp"
#include "hero.hpp"
#include "platform.hpp"
#include "machine.hpp"

auto map = Map(WIDTH, HEIGHT, "./resources");
auto machine = Machine();

Hero hero;
std::vector<Entity *> entities;

void CreateEntities(std::vector<EntityPosition> positions, std::vector<Entity *> &entities)
{
    for (auto position : positions)
    {
        if (position.entityType == MapEncoding::Enemy)
        {
            CreateEnemie(position.position, entities);
        }
        else if (position.entityType == MapEncoding::Platform)
        {
            CreatePlatforms(position.position, entities);
        }
    }
}

void drawCb()
{
    // Drawing map
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    map.Draw();
    // Drawing entities
    for (auto enemy : entities)
    {
        enemy->Draw();
    }

    hero.Draw();

    glutSwapBuffers();
}

void Engine(int time)
{
    auto bricks = machine.FindNearby(hero.GetShape());

    for (auto entity : entities)
    {
        entity->Run(bricks);
        machine.UpdatePosition(entity);
    }
    hero.Run(bricks);

    auto opt_pattern = map.InitDraw(hero.level);
    if (opt_pattern.has_value())
    {
        auto pattern = opt_pattern.value();
        machine.Clear();

        for (auto brick : pattern.bricks)
        {
            machine.AddObject(brick);
        }
        CreateEntities(pattern.positions, entities);
    }

    glutPostRedisplay();
    glutTimerFunc(1000 / FRAME_FREQUENCY, Engine, 0);
}

void specialKey(int key, int x, int y)
{
    hero.HandleClick(key);
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    {
        auto opt_pattern = map.InitDraw(0);

        if (opt_pattern.has_value())
        {
            auto pattern = opt_pattern.value();

            CreateEntities(pattern.positions, entities);

            for (auto brick : pattern.bricks)
            {
                machine.AddObject(brick);
            }
            for (auto entitie : entities)
            {
                machine.AddObject(entitie);
            }

            for (auto position : pattern.positions)
            {
                if (position.entityType == MapEncoding::Hero)
                {
                    hero = Hero(position.position.ix, position.position.iy, 1, 1, 3);
                }
            }
        }
    }

    auto window = Window(SCREEN_WIDTH, SCREEN_HEIGHT);
    window.Init(argc, argv);

    glutTimerFunc(1000 / FRAME_FREQUENCY, Engine, 0);

    window.RegisterSpecialKeyboardCb(specialKey);
    window.Draw(drawCb);

    return 0;
}