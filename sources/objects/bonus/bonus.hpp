#pragma once

#include <GL/glut.h>

#include "../entity/entity.hpp"

class Bonus : virtual public Entity
{
private:
    Texture *_texture;

    auto collisionLeftDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void;
    auto collisionRightDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void;

public:
    BonusType type;

    Bonus(double ix, double iy, BonusType type);

    auto Draw() -> void;
    auto Run(std::unordered_set<Object *> neighbours) -> void;
};

Bonus::Bonus(double ix, double iy, BonusType type) : Entity{
                                                         ix / DELTA_X + 0.1,
                                                         HEIGHT - 1 - iy / DELTA_Y,
                                                         0.8,
                                                         1,
                                                         MapEncoding::Bonus},
                                                     type(type)
{
    if (type == BonusType::BigHit)
    {
        _texture = FIREBALL;
    }
    else if (type == BonusType::OneLife)
    {
        _texture = HEART;
    }
    else if (type == BonusType::Bow)
    {
        _texture = BOW;
    }
    else if (type == BonusType::Arrows)
    {
        _texture = QUIVER;
    }
    else if (type == BonusType::Key)
    {
        _texture = KEY;
    }
    else
    {
        _texture = HEART;
    }
};

auto Bonus::Draw() -> void
{
    glBindTexture(GL_TEXTURE_2D, _texture->GetTexture());
    glEnable(GL_TEXTURE_2D);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);

    glTexCoord2f(0, 0);
    glVertex2f(_x - 1.f, _y - 1.f);

    glTexCoord2f(0, 1);
    glVertex2f(_x - 1.f, _y + DELTA_Y * _sizeY - 1.f);

    glTexCoord2f(1, 1);
    glVertex2f(_x + DELTA_X * _sizeX - 1.f, _y + DELTA_Y * _sizeY - 1.f);

    glTexCoord2f(1, 0);
    glVertex2f(_x + DELTA_X * _sizeX - 1.f, _y - 1.f);

    glEnd();

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
};
auto Bonus::Run(std::unordered_set<Object *> neighbours) -> void {};

auto Bonus::collisionLeftDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void {};
auto Bonus::collisionRightDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void {};
