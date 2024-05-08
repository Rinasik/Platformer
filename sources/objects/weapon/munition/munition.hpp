#pragma once

#include "../../entity/entity.hpp"

class Munition : public Entity
{
protected:
    Direction _direction;

    auto collisionLeftDetected(std::shared_ptr<Object> neighbour, Shape nshape, double &virtualDeltaX) -> void;
    auto collisionRightDetected(std::shared_ptr<Object> neighbour, Shape nshape, double &virtualDeltaX) -> void;

public:
    Entity *owner;

    Munition();
    Munition(double ix, double iy, double sizeX, double sizeY, Direction direction, Entity *owner, MapEncoding type);
};

Munition::Munition(){};

Munition::Munition(double ix, double iy, double sizeX, double sizeY, Direction direction, Entity *owner, MapEncoding type) : Entity{
                                                                                                                                 ix + ((direction == Direction::Right) ? 0.5 : -0.5),
                                                                                                                                 iy - 0.25,
                                                                                                                                 sizeX,
                                                                                                                                 sizeY,
                                                                                                                                 type},
                                                                                                                             _direction(direction), owner(owner)
{
}

auto Munition::collisionLeftDetected(std::shared_ptr<Object> neighbour, Shape nshape, double &virtualDeltaX) -> void
{
    if (neighbour->type == MapEncoding::Brick || neighbour->type == MapEncoding::Platform || neighbour->type == MapEncoding::Chest || neighbour->type == MapEncoding::Box)
    {
        isDestroyed = true;
    }
}
auto Munition::collisionRightDetected(std::shared_ptr<Object> neighbour, Shape nshape, double &virtualDeltaX) -> void
{
    if (neighbour->type == MapEncoding::Brick || neighbour->type == MapEncoding::Platform || neighbour->type == MapEncoding::Chest || neighbour->type == MapEncoding::Box)
    {
        isDestroyed = true;
    }
}