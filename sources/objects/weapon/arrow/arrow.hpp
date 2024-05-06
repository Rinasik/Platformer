#pragma once

#include "../munition/munition.hpp"
#include <GL/glut.h>

class Arrow : virtual public Munition
{
private:
    double _velX;

    Texture *_arrow;

public:
    Arrow();
    Arrow(double ix, double iy, Direction direction, Entity *owner);
    auto Draw() -> void;
    auto Run(std::unordered_set<Object *> neighbours) -> void;
};

Arrow::Arrow(){};
Arrow::Arrow(double ix, double iy, Direction direction, Entity *owner) : Munition{
                                                                             ix,
                                                                             iy,
                                                                             1,
                                                                             0.5,
                                                                             direction,
                                                                             owner,
                                                                             MapEncoding::Arrow}
{
    if (_direction == Direction::Left)
    {
        _velX = -ARROW_VELOCITY;
    }
    else
    {
        _velX = ARROW_VELOCITY;
    }

    _arrow = ARROW;
};

auto Arrow::Draw() -> void
{
    glBindTexture(GL_TEXTURE_2D, _arrow->GetTexture());
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

auto Arrow::Run(std::unordered_set<Object *> neighbours) -> void
{
    double virtualDeltaX = _velX;
    objectsCollisionX(neighbours, virtualDeltaX);

    if (_x + virtualDeltaX <= 0 || _x + virtualDeltaX >= 2)
    {
        isDestroyed = true;
    }

    _x += virtualDeltaX;
}