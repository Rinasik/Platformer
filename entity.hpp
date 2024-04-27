#pragma once

#include <set>

#include "utils.hpp"
#include "constants.hpp"

class Entity
{
protected:
    double _x = 0;
    double _y = 0;

    double _velX = 0;
    double _velY = 0;

    int _sizeX;
    int _sizeY;

    auto windowBoundsCollision(double &virtualDeltaX, double &virtualDeltaY) -> void;
    auto entitiesAndMapCollisionX(std::set<Entity *> neighbours, double &virtualDeltaX, double &virtualDeltaY) -> void;

    virtual auto collisionLeftDetected(Entity *neighbour, Shape nshape, double &virtualDeltaX) -> void = 0;
    virtual auto collisionRightDetected(Entity *neighbour, Shape nshape, double &virtualDeltaX) -> void = 0;

public:
    MapEncoding type;

    Entity(){};
    Entity(int ix, int iy, int sizeX, int sizeY, MapEncoding type);

    auto GetShape() -> Shape;
    virtual auto Draw() -> void = 0;
    virtual auto Run(std::set<Entity *> neighbours) -> void = 0;
};

Entity::Entity(int ix, int iy, int sizeX, int sizeY, MapEncoding type) : _x(multiply(ix, DELTA_X)), _y(multiply(HEIGHT - iy - 1, DELTA_Y)), _sizeX(sizeX), _sizeY(sizeY), type(type) {}

inline auto Entity::GetShape() -> Shape
{
    return Shape{add(multiply(_sizeY, DELTA_Y), _y), _y, _x, add(multiply((_sizeX), DELTA_X), _x)};
}

auto Entity::windowBoundsCollision(double &virtualDeltaX, double &virtualDeltaY) -> void
{
    auto shape = GetShape();

    if (shape.right + virtualDeltaX >= 2.0)
    {
        _x = 2.0 - DELTA_X * _sizeX;

        _velX = 0;
        virtualDeltaX = 0;
    }
    if (shape.left + virtualDeltaX <= 0)
    {
        _x = 0;

        _velX = 0;
        virtualDeltaX = 0;
    }

    if (shape.bottom + virtualDeltaY <= 0)
    {
        _y = 0;

        _velY = 0;
        virtualDeltaY = 0;
    }

    if (shape.top + virtualDeltaY >= 2.0)
    {
        _y = 2.0 - DELTA_Y * _sizeX;

        _velY = -_velY / 2;
        virtualDeltaY = 0;
    }
}

auto Entity::entitiesAndMapCollisionX(std::set<Entity *> neighbours, double &virtualDeltaX, double &virtualDeltaY) -> void
{
    auto shape = GetShape();

    for (auto neighbour : neighbours)
    {
        auto nShape = neighbour->GetShape();

        auto isAboveThanBottom = nShape.top - shape.bottom >= EPSILON;
        auto isBelowThanTop = (nShape.bottom - shape.top) <= -EPSILON;

        if (isAboveThanBottom && isBelowThanTop)
        {
            // Движется вправо
            if ((nShape.left >= shape.left) && (nShape.left <= (shape.right + virtualDeltaX)))
            {
                collisionRightDetected(neighbour, nShape, virtualDeltaX);
            }
            // Движется влево
            else if (nShape.right <= (shape.right) && nShape.right >= (shape.left + virtualDeltaX))
            {
                collisionLeftDetected(neighbour, nShape, virtualDeltaX);
            }
        }
    }
}