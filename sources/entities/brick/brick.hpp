#pragma once

#include "../entity/entity.hpp"

class Brick : virtual public Entity
{
private:
    auto collisionLeftDetected(Entity *neighbour, Shape nshape, double &virtualDeltaX) -> void;
    auto collisionRightDetected(Entity *neighbour, Shape nshape, double &virtualDeltaX) -> void;

public:
    Brick(int ix, int iy);

    auto Run(std::set<Entity *> neighbours) -> void;
    auto Draw() -> void;
};


Brick::Brick(int ix, int iy) : Entity{
                                   ix,
                                   iy,
                                   1,
                                   1,
                                   MapEncoding::Brick}
{
}

auto Brick::Run(std::set<Entity *> neighbours) -> void {}
auto Brick::Draw() -> void {}
auto Brick::collisionLeftDetected(Entity *neighbour, Shape nshape, double &virtualDeltaX) -> void {}
auto Brick::collisionRightDetected(Entity *neighbour, Shape nshape, double &virtualDeltaX) -> void {}