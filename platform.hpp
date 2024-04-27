#pragma once

#include <Gl/glut.h>
#include "entity.hpp"

class Platform : virtual public Entity
{
private:
    auto collisionLeftDetected(Entity *neighbour, Shape nshape, double &virtualDeltaX) -> void;
    auto collisionRightDetected(Entity *neighbour, Shape nshape, double &virtualDeltaX) -> void;

public:
    Platform(int ix, int iy, int sizeX);

    double lastDeltaX;

    auto Draw() -> void;
    auto Run(std::vector<Entity *> neighbours) -> void;
};

Platform::Platform(int ix, int iy, int sizeX) : Entity{
                                                    ix,
                                                    iy,
                                                    sizeX,
                                                    1,
                                                    MapEncoding::Platform}
{
    _velX = 0.0025f;
}

auto Platform::Run(std::vector<Entity *> neighbours) -> void
{
    _x += _velX;
    lastDeltaX = _velX;
}

auto Platform::Draw() -> void
{
    glColor3f(1.0f, 0.0f, 1.0f);

    glBegin(GL_QUADS);

    glVertex2f(_x - 1.f, _y - 1.f);
    glVertex2f(_x - 1.f, _y + DELTA_Y * _sizeY - 1.f);
    glVertex2f(_x + DELTA_X * _sizeX - 1.f, _y + DELTA_Y * _sizeY - 1.f);
    glVertex2f(_x + DELTA_X * _sizeX - 1.f, _y - 1.f);

    glEnd();
}

void CreatePlatforms(Position position, std::vector<Entity *> &entities)
{
    entities.push_back(new Platform(position.ix, position.iy, 2));
}

auto Platform::collisionLeftDetected(Entity *neighbour, Shape nshape, double &virtualDeltaX) -> void {}
auto Platform::collisionRightDetected(Entity *neighbour, Shape nshape, double &virtualDeltaX) -> void {}