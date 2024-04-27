#pragma once

#include "entity.hpp"

class Exit : virtual public Entity
{
private:
    auto collisionLeftDetected(Entity *neighbour, Shape nshape, double &virtualDeltaX) -> void;
    auto collisionRightDetected(Entity *neighbour, Shape nshape, double &virtualDeltaX) -> void;

public:
    int mapNumber;

    Exit(int ix, int iy, int mapNumber);

    auto Run(std::vector<Entity *> neighbours) -> void;
    auto Draw() -> void;
};

Exit::Exit(int ix, int iy, int mapNumber) : Entity{
                                                ix,
                                                iy,
                                                1,
                                                1,
                                                MapEncoding::Exit}

{
    this->mapNumber = mapNumber;
}

auto Exit::Run(std::vector<Entity *> neighbours) -> void {}
auto Exit::Draw() -> void {}
auto Exit::collisionLeftDetected(Entity *neighbour, Shape nshape, double &virtualDeltaX) -> void {}
auto Exit::collisionRightDetected(Entity *neighbour, Shape nshape, double &virtualDeltaX) -> void {}