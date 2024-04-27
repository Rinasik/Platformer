#pragma once

#include "entity.hpp"
#include "utils.hpp"
#include "constants.hpp"
#include <GL/glut.h>

class Enemy : virtual public Entity
{
private:
    double _velX = 0;
    double _velY = 0;

    int _lives;
    bool _alive = true;

    auto mapCollision(std::vector<MapEncoding> mapPattern) -> void;

    auto collisionLeftDetected(Entity *neighbour, Shape nshape, double &virtualDeltaX) -> void;
    auto collisionRightDetected(Entity *neighbour, Shape nshape, double &virtualDeltaX) -> void;

public:
    Enemy(int ix, int iy, int sizeX, int sizeY, int lives);

    auto GetShape() -> Shape;
    auto Run(std::vector<Entity *> neighbours) -> void;
    auto Draw() -> void;
};

Enemy::Enemy(int ix, int iy, int sizeX, int sizeY, int lives) : Entity{
                                                                    ix,
                                                                    iy,
                                                                    sizeX,
                                                                    sizeY,
                                                                    MapEncoding::Enemy},
                                                                _lives(lives)
{
}

auto Enemy::Draw() -> void
{
    if (!_alive)
    {
        return;
    }

    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_QUADS);

    glVertex2f(_x - 1.f, _y - 1.f);
    glVertex2f(_x - 1.f, _y + DELTA_Y * _sizeY - 1.f);
    glVertex2f(_x + DELTA_X * _sizeX - 1.f, _y + DELTA_Y * _sizeY - 1.f);
    glVertex2f(_x + DELTA_X * _sizeX - 1.f, _y - 1.f);

    glEnd();
}

auto Enemy::Run(std::vector<Entity *> neighbours) -> void
{
    if (!_alive)
    {
        return;
    }
}

auto Enemy::mapCollision(std::vector<MapEncoding> mapPattern) -> void
{
}

void CreateEnemie(Position position, std::vector<Entity *> &entities)
{
    entities.push_back(new Enemy(position.ix, position.iy, 1, 1, 2));
}

auto Enemy::collisionLeftDetected(Entity *neighbour, Shape nshape, double &virtualDeltaX) -> void {}
auto Enemy::collisionRightDetected(Entity *neighbour, Shape nshape, double &virtualDeltaX) -> void {}