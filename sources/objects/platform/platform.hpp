#pragma once

#include <GL/glut.h>
#include "../entity/entity.hpp"

#include "../../texture/texture.hpp"

class Platform : virtual public Entity
{
private:
    auto collisionLeftDetected(std::shared_ptr<Object> neighbour, Shape nshape, double &virtualDeltaX) -> void;
    auto collisionRightDetected(std::shared_ptr<Object> neighbour, Shape nshape, double &virtualDeltaX) -> void;

public:
    Platform(int ix, int iy, int sizeX);

    double lastDeltaX;

    auto Draw() -> void;
    auto Run(std::unordered_set<std::shared_ptr<Object>> neighbours) -> void;
};

void CreatePlatforms(Position position, std::vector<std::shared_ptr<Entity>> &entities)
{
    entities.push_back(std::shared_ptr<Platform>(new Platform(position.ix, position.iy, 2)));
}

Platform::Platform(int ix, int iy, int sizeX) : Entity{
                                                    (double)ix,
                                                    (double)iy,
                                                    (double)sizeX,
                                                    1,
                                                    MapEncoding::Platform}
{
    _velX = 0.0025f;
}

auto Platform::Run(std::unordered_set<std::shared_ptr<Object>> neighbours) -> void
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

auto Platform::collisionLeftDetected(std::shared_ptr<Object> neighbour, Shape nShape, double &virtualDeltaX) -> void
{
    if (neighbour->type == MapEncoding::Brick)
    {
        _velX = -_velX;
        virtualDeltaX = 0;
    }
}
auto Platform::collisionRightDetected(std::shared_ptr<Object> neighbour, Shape nShape, double &virtualDeltaX) -> void
{
    if (neighbour->type == MapEncoding::Brick)
    {
        _velX = -_velX;
        virtualDeltaX = 0;
    }
}