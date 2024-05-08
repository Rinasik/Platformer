#pragma once

#include "../enemy/enemy.hpp"
#include <functional>

class Monster : virtual public Enemy
{
private:
    bool _isFalling = false;

    std::optional<std::shared_ptr<Hit>> _hit = std::nullopt;
    std::function<void(std::shared_ptr<Entity>)> _addHit;

public:
    Monster(double ix, double iy, std::function<void(std::shared_ptr<Entity>)> addHit);

    auto Run(std::unordered_set<std::shared_ptr<Object>> neighbours) -> void;
    auto Draw() -> void;

    auto GetBonus() -> std::optional<std::shared_ptr<Bonus>>;
};

Monster::Monster(double ix, double iy, std::function<void(std::shared_ptr<Entity>)> addHit) : Enemy{
                                                                                                  ix,
                                                                                                  iy,
                                                                                                  1, 1, 5,
                                                                                                  std::vector<Direction>({
                                                                                                      Direction::Right,
                                                                                                      Direction::Up,
                                                                                                      Direction::Right,
                                                                                                      Direction::Left,
                                                                                                      Direction::Left,
                                                                                                      Direction::Left,
                                                                                                      Direction::Up,
                                                                                                      Direction::Left,
                                                                                                      Direction::Right,
                                                                                                      Direction::Right,
                                                                                                      Direction::Up,
                                                                                                  }),
                                                                                                  MapEncoding::Monster}
{
    _velX = ENEMY_X_VELOCITY;
    _addHit = addHit;
}

void CreateMonster(Position position, std::unordered_set<std::shared_ptr<Entity>> &entities, std::function<void(std::shared_ptr<Entity>)> addHit)
{
    entities.emplace(new Monster(position.ix, position.iy, addHit));
}

auto Monster::Draw() -> void
{
    if (!_lives)
    {
        return;
    }

    glColor4f(0.87f, 0.0f, 0.32f, 1.0f);

    glBegin(GL_QUADS);

    glVertex2f(_x - 1.f, _y - 1.f);
    glVertex2f(_x - 1.f, _y + DELTA_Y * _sizeY - 1.f);
    glVertex2f(_x + DELTA_X * _sizeX - 1.f, _y + DELTA_Y * _sizeY - 1.f);
    glVertex2f(_x + DELTA_X * _sizeX - 1.f, _y - 1.f);

    glEnd();
}

auto Monster::Run(std::unordered_set<std::shared_ptr<Object>> neighbours) -> void
{
    if (_hit.has_value())
    {
        if (_hit.value()->isDestroyed)
        {
            _hit = std::nullopt;
        }
    }

    if (!_lives)
    {
        isDestroyed = true;
        return;
    }

    if (!_hit.has_value())
    {
        for (auto &neighbour : neighbours)
        {
            if (neighbour->type == MapEncoding::Hero)
            {
                Shape shape = GetShape();
                Shape nShape = neighbour->GetShape();
                if (shape.right - nShape.right >= 0 && _velX <= 0)
                {
                    _hit = std::shared_ptr<Hit>(new Hit(_x / DELTA_X, HEIGHT - 1 - _y / DELTA_Y, Direction::Left, 2, this));
                    _addHit(_hit.value());
                }
                else if (nShape.left - shape.left >= 0 && _velX >= 0)
                {
                    _hit = std::shared_ptr<Hit>(new Hit(_x / DELTA_X, HEIGHT - 1 - _y / DELTA_Y, Direction::Right, 2, this));
                    _addHit(_hit.value());
                }
            }
        }
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

auto Monster::GetBonus() -> std::optional<std::shared_ptr<Bonus>>
{
    int probability = std::rand() % 10;

    if (probability <= 2)
    {
        return std::shared_ptr<Bonus>(new Bonus(_x, _y, BonusType::OneLife));
    }
    else if (probability >= 2 && probability <= 4)
    {
        return std::shared_ptr<Bonus>(new Bonus(_x, _y, BonusType::MaxLives));
    }

    return std::nullopt;
}