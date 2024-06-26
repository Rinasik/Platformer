#pragma once

#include <unordered_set>

#include "../object/object.hpp"

class Entity : public Object
{
protected:
    double _velX = 0;
    double _velY = 0;

    auto windowBoundsCollision(double &virtualDeltaX, double &virtualDeltaY) -> void;
    auto objectsCollisionX(std::unordered_set<std::shared_ptr<Object>> neighbours, double &virtualDeltaX) -> void;

    virtual auto collisionLeftDetected(std::shared_ptr<Object> neighbour, Shape nshape, double &virtualDeltaX) -> void = 0;
    virtual auto collisionRightDetected(std::shared_ptr<Object> neighbour, Shape nshape, double &virtualDeltaX) -> void = 0;

public:
    bool isDestroyed = false;

    Entity(){};
    Entity(double x, double y, double sizeX, double sizeY, MapEncoding type);

    virtual auto Draw() -> void = 0;
    virtual auto Run(std::unordered_set<std::shared_ptr<Object>> neighbours) -> void = 0;
};

Entity::Entity(double x, double y, double sizeX, double sizeY, MapEncoding type) : Object{x,
                                                                                          y,
                                                                                          sizeX,
                                                                                          sizeY,
                                                                                          type} {}

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

auto Entity::objectsCollisionX(std::unordered_set<std::shared_ptr<Object>> neighbours, double &virtualDeltaX) -> void
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