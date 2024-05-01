#pragma once

#include <GL/glut.h>

#include "../entity/entity.hpp"

class Bonus : virtual public Entity
{
private:
    auto collisionLeftDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void;
    auto collisionRightDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void;

public:
    BonusType type;

    Bonus(double ix, double iy, BonusType type);

    auto Draw() -> void;
    auto Run(std::unordered_set<Object *> neighbours) -> void;
};

Bonus::Bonus(double ix, double iy, BonusType type) : Entity{
                                                         ix / DELTA_X + 0.5,
                                                         HEIGHT - 1 - iy / DELTA_Y,
                                                         0.3,
                                                         0.5,
                                                         MapEncoding::Bonus}, type(type) {};

auto Bonus::Draw() -> void
{
    glColor4f(0.32f, 0.8f, 0.87f, 1.0f);

    glBegin(GL_QUADS);

    glVertex2f(_x - 1.f, _y - 1.f);
    glVertex2f(_x - 1.f, _y + DELTA_Y * _sizeY - 1.f);
    glVertex2f(_x + DELTA_X * _sizeX - 1.f, _y + DELTA_Y * _sizeY - 1.f);
    glVertex2f(_x + DELTA_X * _sizeX - 1.f, _y - 1.f);

    glEnd();
};
auto Bonus::Run(std::unordered_set<Object *> neighbours) -> void {};

auto Bonus::collisionLeftDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void {};
auto Bonus::collisionRightDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void {};
