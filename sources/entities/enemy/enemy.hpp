#pragma once

#include <GL/glut.h>
#include "../entity/entity.hpp"

class Enemy : virtual public Entity
{
private:
    double _velX = 0;
    double _velY = 0;

    int _lives;

    auto mapCollision(std::set<MapEncoding> mapPattern) -> void;

    auto collisionLeftDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void;
    auto collisionRightDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void;

public:
    Enemy(int ix, int iy, int sizeX, int sizeY, int lives);

    auto Run(std::set<Object *> neighbours) -> void;
    auto Draw() -> void;
};

void CreateEnemie(Position position, std::vector<Entity *> &entities)
{
    entities.push_back(new Enemy(position.ix, position.iy, 1, 1, 2));
}

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
    if (!_lives)
    {
        return;
    }

    glColor3f(0.87f, 0.0f, 0.32f);

    glBegin(GL_QUADS);

    glVertex2f(_x - 1.f, _y - 1.f);
    glVertex2f(_x - 1.f, _y + DELTA_Y * _sizeY - 1.f);
    glVertex2f(_x + DELTA_X * _sizeX - 1.f, _y + DELTA_Y * _sizeY - 1.f);
    glVertex2f(_x + DELTA_X * _sizeX - 1.f, _y - 1.f);

    glEnd();
}

auto Enemy::Run(std::set<Object *> neighbours) -> void
{
    if (!_lives)
    {
        return;
    }

    _x += _velX;
}

auto Enemy::mapCollision(std::set<MapEncoding> mapPattern) -> void
{
}

auto Enemy::collisionLeftDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void {}
auto Enemy::collisionRightDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void {}