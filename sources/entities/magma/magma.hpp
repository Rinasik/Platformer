#pragma once

#include "../entity/entity.hpp"

class Magma : virtual public Entity
{
private:
    auto collisionLeftDetected(Entity *neighbour, Shape nshape, double &virtualDeltaX) -> void;
    auto collisionRightDetected(Entity *neighbour, Shape nshape, double &virtualDeltaX) -> void;

public:
    Magma(int ix, int iy);

    auto Run(std::set<Entity *> neighbours) -> void;
    auto Draw() -> void;
};

Magma::Magma(int ix, int iy) : Entity{
                                   ix,
                                   iy,
                                   1,
                                   1,
                                   MapEncoding::Magma}
{
}

auto Magma::Run(std::set<Entity *> neighbours) -> void {}
auto Magma::Draw() -> void {}
auto Magma::collisionLeftDetected(Entity *neighbour, Shape nshape, double &virtualDeltaX) -> void {}
auto Magma::collisionRightDetected(Entity *neighbour, Shape nshape, double &virtualDeltaX) -> void {}