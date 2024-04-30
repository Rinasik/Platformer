#pragma once

#include "../entity/entity.hpp"
#include <GL/glut.h>

class Hit : virtual public Entity
{
private:
    double _velX;
    double _initialX;

    Direction _direction;

    Texture _fireball;

    auto collisionLeftDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void;
    auto collisionRightDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void;

public:
    Hit();
    Hit(double ix, double iy, Direction direction);
    auto Draw() -> void;
    auto Run(std::unordered_set<Object *> neighbours) -> void;
};

Hit::Hit(){};
Hit::Hit(double ix, double iy, Direction direction) : Entity{
                                                          ix + ((direction == Direction::Right) ? 0.5 : -0.5),
                                                          iy - 0.25,
                                                          1,
                                                          0.5,
                                                          MapEncoding::Hit},
                                                      _direction(direction)
{
    if (_direction == Direction::Left)
    {
        _velX = -HIT_VELOCITY;
    }
    else
    {
        _velX = HIT_VELOCITY;
    }
    _initialX = _x;

    _fireball = Texture("images/Fireball.png", true);
};

auto Hit::Draw() -> void
{
    glBindTexture(GL_TEXTURE_2D, _fireball.GetTexture());
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
    if (_direction == Direction::Right)
    {
        glTexCoord2f(0, 0);
        glVertex2f(_x - 1.f, _y - 1.f);

        glTexCoord2f(0, 1);
        glVertex2f(_x - 1.f, _y + DELTA_Y * _sizeY - 1.f);

        glTexCoord2f(1, 1);
        glVertex2f(_x + DELTA_X * _sizeX - 1.f, _y + DELTA_Y * _sizeY - 1.f);

        glTexCoord2f(1, 0);
        glVertex2f(_x + DELTA_X * _sizeX - 1.f, _y - 1.f);
    }
    else
    {
        glTexCoord2f(1, 0);
        glVertex2f(_x - 1.f, _y - 1.f);

        glTexCoord2f(1, 1);
        glVertex2f(_x - 1.f, _y + DELTA_Y * _sizeY - 1.f);

        glTexCoord2f(0, 1);
        glVertex2f(_x + DELTA_X * _sizeX - 1.f, _y + DELTA_Y * _sizeY - 1.f);

        glTexCoord2f(0, 0);
        glVertex2f(_x + DELTA_X * _sizeX - 1.f, _y - 1.f);
    }

    glEnd();

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

auto Hit::Run(std::unordered_set<Object *> neighbours) -> void
{
    double virtualDeltaX = _velX;
    objectsCollisionX(neighbours, virtualDeltaX);

    _x += virtualDeltaX;

    if (abs((_x - _initialX) / DELTA_X) >= 1)
    {
        isDestroyed = true;
    }
}

auto Hit::collisionLeftDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void
{
    if (neighbour->type == MapEncoding::Brick || neighbour->type == MapEncoding::Platform)
    {
        isDestroyed = true;
    }
}
auto Hit::collisionRightDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void
{
    if (neighbour->type == MapEncoding::Brick || neighbour->type == MapEncoding::Platform)
    {
        isDestroyed = true;
    }
}