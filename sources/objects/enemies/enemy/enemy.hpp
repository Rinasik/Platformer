#pragma once

#include "../../entity/entity.hpp"
#include "../../bonus/bonus.hpp"
#include "../../weapon/hit/hit.hpp"
#include "../../weapon/arrow/arrow.hpp"

class Enemy : public Entity
{
protected:
    bool _isInvisible = false;

    double _prevX = 0;
    double _initialY = 0;

    std::vector<Direction> _pattern;
    int _currentStep = 0;

    int _lives;

    auto collisionLeftDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void;
    auto collisionRightDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void;

public:
    Enemy(double ix, double iy, int sizeX, int sizeY, int lives, std::vector<Direction> pattern, MapEncoding type);

    virtual auto GetBonus() -> std::optional<Bonus *> = 0;
};

Enemy::Enemy(double ix, double iy, int sizeX, int sizeY, int lives, std::vector<Direction> pattern, MapEncoding type) : Entity{
                                                                                                                            ix,
                                                                                                                            iy,
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
    if (!_isInvisible && neighbour->type == MapEncoding::Hit)
    {
        Hit *hit = dynamic_cast<Hit *>(neighbour);

        if (this != hit->owner)
        {
            _lives--;
            hit->isDestroyed = true;
        }
    }
    else if (!_isInvisible && neighbour->type == MapEncoding::Arrow)
    {
        Arrow *arrow = dynamic_cast<Arrow *>(neighbour);
        if (this != arrow->owner)
        {
            _lives--;
            arrow->isDestroyed = true;
        }
    }
}

auto Enemy::collisionRightDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void
{
    if (!_isInvisible && neighbour->type == MapEncoding::Hit)
    {
        Hit *hit = dynamic_cast<Hit *>(neighbour);

        if (this != hit->owner)
        {
            _lives--;
            hit->isDestroyed = true;
        }
    }
    else if (!_isInvisible && neighbour->type == MapEncoding::Arrow)
    {
        Arrow *arrow = dynamic_cast<Arrow *>(neighbour);
        if (this != arrow->owner)
        {
            _lives--;
            arrow->isDestroyed = true;
        }
    }
}

bool IsEnemy(Object *object)
{
    return object->type == MapEncoding::Warrior || object->type == MapEncoding::Archer || object->type == MapEncoding::Monster || object->type == MapEncoding::Jumper || object->type == MapEncoding::Boss;
}