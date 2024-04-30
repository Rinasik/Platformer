#pragma once

#include <GL/glut.h>
#include "../entity/entity.hpp"

class Platform : virtual public Entity
{
private:
    auto collisionLeftDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void;
    auto collisionRightDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void;

public:
    Platform(int ix, int iy, int sizeX);

    double lastDeltaX;

    auto Draw() -> void;
    auto Run(std::set<Object *> neighbours) -> void;
};

void CreatePlatforms(Position position, std::vector<Entity *> &entities)
{
    entities.push_back(new Platform(position.ix, position.iy, 2));
}

Platform::Platform(int ix, int iy, int sizeX) : Entity{
                                                    ix,
                                                    iy,
                                                    sizeX,
                                                    1,
                                                    MapEncoding::Platform}
{
    _velX = 0.0025f;
}

auto Platform::Run(std::set<Object *> neighbours) -> void
{
    auto deltaX = _velX;
    objectsCollisionX(neighbours, deltaX);

    _x += deltaX;
    lastDeltaX = deltaX;
}

auto Platform::Draw() -> void
{
    glColor4f(1.0f, 0.0f, 1.0f, 1);

    glBegin(GL_QUADS);

    glVertex2f(_x - 1.f, _y - 1.f);
    glVertex2f(_x - 1.f, _y + DELTA_Y * _sizeY - 1.f);
    glVertex2f(_x + DELTA_X * _sizeX - 1.f, _y + DELTA_Y * _sizeY - 1.f);
    glVertex2f(_x + DELTA_X * _sizeX - 1.f, _y - 1.f);

    glEnd();
}

auto Platform::collisionLeftDetected(Object *neighbour, Shape nShape, double &virtualDeltaX) -> void
{
    if (neighbour->type == MapEncoding::Brick)
    {
        _velX = -_velX;
        virtualDeltaX = 0;
    }
}
auto Platform::collisionRightDetected(Object *neighbour, Shape nShape, double &virtualDeltaX) -> void
{
    if (neighbour->type == MapEncoding::Brick)
    {
        _velX = -_velX;
        virtualDeltaX = 0;
    }
}