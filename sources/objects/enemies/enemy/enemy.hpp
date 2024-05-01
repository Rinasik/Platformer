#pragma once

#include "../../entity/entity.hpp"
#include "../../bonus/bonus.hpp"
#include "../../hit/hit.hpp"

class Enemy : public Entity
{
protected:
    double _prevX = 0;
    double _initialY = 0;

    std::vector<Direction> _pattern;
    int _currentStep = 0;

    int _lives;

    auto collisionLeftDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void;
    auto collisionRightDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void;

public:
    Enemy(int ix, int iy, int sizeX, int sizeY, int lives, std::vector<Direction> pattern, MapEncoding type);

    virtual auto GetBonus() -> std::optional<Bonus *> = 0;
};

Enemy::Enemy(int ix, int iy, int sizeX, int sizeY, int lives, std::vector<Direction> pattern, MapEncoding type) : Entity{
                                                                                                                      (double)ix,
                                                                                                                      (double)iy,
                                                                                                                      (double)sizeX,
                                                                                                                      (double)sizeY,
                                                                                                                      type},
                                                                                                                  _pattern(pattern), _lives(lives)
{
    _prevX = _x;
    _initialY = _y;
}

auto Enemy::collisionLeftDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void
{
    if (neighbour->type == MapEncoding::Hit)
    {
        _lives--;
        Hit *hit = dynamic_cast<Hit *>(neighbour);
        hit->isDestroyed = true;
    }
}

auto Enemy::collisionRightDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void
{
    if (neighbour->type == MapEncoding::Hit)
    {
        _lives--;
        Hit *hit = dynamic_cast<Hit *>(neighbour);
        hit->isDestroyed = true;
    }
}

bool IsEnemy(Entity *entity)
{
    return entity->type == MapEncoding::Warrior || entity->type == MapEncoding::Archer || entity->type == MapEncoding::Monster || entity->type == MapEncoding::Jumper;
}