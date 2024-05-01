#pragma once

#include "../enemy/enemy.hpp"

class Jumper : virtual public Enemy
{
private:
    bool _isFalling = false;

public:
    Jumper(int ix, int iy);

    auto Run(std::unordered_set<Object *> neighbours) -> void;
    auto Draw() -> void;

    auto GetBonus() -> std::optional<Bonus *>;
};

Jumper::Jumper(int ix, int iy) : Enemy{
                                     ix,
                                     iy,
                                     1, 1, 2,
                                     std::vector<Direction>({Direction::Right, Direction::Up, Direction::Right, Direction::Left, Direction::Left, Direction::Left, Direction::Up, Direction::Left, Direction::Right, Direction::Right}),
                                     MapEncoding::Jumper}
{
    _velX = ENEMY_X_VELOCITY;
}

void CreateJumper(Position position, std::unordered_set<Entity *> &entities)
{
    entities.emplace(new Jumper(position.ix, position.iy));
}

auto Jumper::Draw() -> void
{
    if (!_lives)
    {
        return;
    }

    glColor4f(0.32f, 0.8f, 0.87f, 1.0f);

    glBegin(GL_QUADS);

    glVertex2f(_x - 1.f, _y - 1.f);
    glVertex2f(_x - 1.f, _y + DELTA_Y * _sizeY - 1.f);
    glVertex2f(_x + DELTA_X * _sizeX - 1.f, _y + DELTA_Y * _sizeY - 1.f);
    glVertex2f(_x + DELTA_X * _sizeX - 1.f, _y - 1.f);

    glEnd();
}

auto Jumper::Run(std::unordered_set<Object *> neighbours) -> void
{
    if (!_lives)
    {
        isDestroyed = true;
        return;
    }

    auto initialVelY = _velY;
    if (_isFalling)
    {
        if ((_velY + Y_ACC) >= -MAX_Y_VELOCITY)
        {
            _velY -= Y_ACC;
        }
        else
        {
            _velY = -MAX_Y_VELOCITY;
        }
    }

    auto virtualDeltaY = (_velY + initialVelY) / 20;
    auto virtualDeltaX = _velX;

    double currentPosX = abs((_x - _prevX) / DELTA_X);
    double isLanded = _isFalling && (_y + virtualDeltaY) <= (_initialY + EPSILON);

    if (currentPosX >= 1 || isLanded)
    {
        virtualDeltaY = 0;
        _y = _initialY;
        _isFalling = false;
        _velY = 0;

        _prevX = _x;

        int _nextStep = (_currentStep + 1) % _pattern.size();

        if (_pattern[_nextStep] == Direction::Up)
        {
            _velY = DELTA_Y_VELOCITY;
            _isFalling = true;
            _velX = 0;
        }
        else
        {
            if (_pattern[_nextStep] == Direction::Left)
            {
                _velX = -ENEMY_X_VELOCITY;
            }
            else
            {
                _velX = ENEMY_X_VELOCITY;
            }
        }

        _currentStep = _nextStep;
    }

    objectsCollisionX(neighbours, virtualDeltaX);

    _x += virtualDeltaX;
    _y += virtualDeltaY;
}

auto Jumper::GetBonus() -> std::optional<Bonus *>
{
    if (std::rand() % 10 <= 1)
    {
        return new Bonus(_x, _y, BonusType::OneLife);
    }

    return std::nullopt;
}
