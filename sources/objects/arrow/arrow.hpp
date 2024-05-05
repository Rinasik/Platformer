#pragma once

#include "../entity/entity.hpp"
#include <GL/glut.h>

class Arrow : virtual public Entity
{
private:
    double _velX;

    Direction _direction;

    Texture *_arrow;

    auto collisionLeftDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void;
    auto collisionRightDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void;

public:
    Entity *owner;

    Arrow();
    Arrow(double ix, double iy, Direction direction, Entity *owner);
    auto Draw() -> void;
    auto Run(std::unordered_set<Object *> neighbours) -> void;
};

Arrow::Arrow(){};
Arrow::Arrow(double ix, double iy, Direction direction, Entity *owner) : Entity{
                                                                             ix + ((direction == Direction::Right) ? 0.5 : -0.5),
                                                                             iy - 0.25,
                                                                             1,
                                                                             0.5,
                                                                             MapEncoding::Arrow},
                                                                         _direction(direction), owner(owner)
{
    if (_direction == Direction::Left)
    {
        _velX = -HIT_VELOCITY;
    }
    else
    {
        _velX = HIT_VELOCITY;
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

auto Arrow::collisionLeftDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void
{
    if (neighbour->type == MapEncoding::Brick || neighbour->type == MapEncoding::Platform || neighbour->type == MapEncoding::Chest || neighbour->type == MapEncoding::Box)
    {
        isDestroyed = true;
    }
}

auto Arrow::collisionRightDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void
{
    if (neighbour->type == MapEncoding::Brick || neighbour->type == MapEncoding::Platform || neighbour->type == MapEncoding::Chest || neighbour->type == MapEncoding::Box)
    {
        isDestroyed = true;
    }
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