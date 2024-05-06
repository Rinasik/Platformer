#pragma once

#include "../munition/munition.hpp"
#include <GL/glut.h>

class Hit : virtual public Munition
{
private:
    double _initialX;
    int _distance;

    Texture *_fireball;

public:
    Hit();
    Hit(double ix, double iy, Direction direction, int distance, Entity *owner);
    auto Draw() -> void;
    auto Run(std::unordered_set<Object *> neighbours) -> void;
};

Hit::Hit(){};
Hit::Hit(double ix, double iy, Direction direction, int distance, Entity *owner) : Munition{
                                                                                       ix,
                                                                                       iy,
                                                                                       1,
                                                                                       0.5,
                                                                                       direction,
                                                                                       owner,
                                                                                       MapEncoding::Hit},
                                                                                   _distance(distance)
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

    _fireball = FIREBALL;
};

auto Hit::Draw() -> void
{
    glBindTexture(GL_TEXTURE_2D, _fireball->GetTexture());
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

    if (abs((_x - _initialX) / DELTA_X) >= _distance)
    {
        isDestroyed = true;
    }
}
