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

    auto collisionLeftDetected(std::shared_ptr<Object> neighbour, Shape nshape, double &virtualDeltaX) -> void;
    auto collisionRightDetected(std::shared_ptr<Object> neighbour, Shape nshape, double &virtualDeltaX) -> void;

public:
    Enemy(double ix, double iy, int sizeX, int sizeY, int lives, std::vector<Direction> pattern, MapEncoding type);

    virtual auto GetBonus() -> std::optional<std::shared_ptr<Bonus>> = 0;
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

auto Enemy::collisionLeftDetected(std::shared_ptr<Object> neighbour, Shape nshape, double &virtualDeltaX) -> void
{
    if (!_isInvisible && neighbour->type == MapEncoding::Hit)
    {
        auto hit = std::dynamic_pointer_cast<Hit>(neighbour);

        if (this != hit->owner)
        {
            _lives--;
            hit->isDestroyed = true;
        }
    }
    else if (!_isInvisible && neighbour->type == MapEncoding::Arrow)
    {
        auto arrow = std::dynamic_pointer_cast<Arrow>(neighbour);
        if (this != arrow->owner)
        {
            _lives--;
            arrow->isDestroyed = true;
        }
    }
}

auto Enemy::collisionRightDetected(std::shared_ptr<Object> neighbour, Shape nshape, double &virtualDeltaX) -> void
{
    if (!_isInvisible && neighbour->type == MapEncoding::Hit)
    {
        auto hit = std::dynamic_pointer_cast<Hit>(neighbour);

        if (this != hit->owner)
        {
            _lives--;
            hit->isDestroyed = true;
        }
    }
    else if (!_isInvisible && neighbour->type == MapEncoding::Arrow)
    {
        auto arrow = std::dynamic_pointer_cast<Arrow>(neighbour);
        if (this != arrow->owner)
        {
            _lives--;
            arrow->isDestroyed = true;
        }
    }
}

bool IsEnemy(std::shared_ptr<Object> object)
{
    return object->type == MapEncoding::Warrior || object->type == MapEncoding::Archer || object->type == MapEncoding::Monster || object->type == MapEncoding::Jumper || object->type == MapEncoding::Boss;
}