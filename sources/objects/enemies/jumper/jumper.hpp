#pragma once

#include "../enemy/enemy.hpp"

class Jumper : virtual public Enemy
{
private:
    bool _isFalling = false;
    Texture *_jumper;

public:
    Jumper(int ix, int iy);

    auto Run(std::unordered_set<std::shared_ptr<Object>> neighbours) -> void;
    auto Draw() -> void;

    auto GetBonus() -> std::optional<std::shared_ptr<Bonus>>;
};

Jumper::Jumper(int ix, int iy) : Enemy{
                                     (double)ix,
                                     (double)iy,
                                     1, 1, 2,
                                     std::vector<Direction>({Direction::Right, Direction::Up, Direction::Right, Direction::Left, Direction::Left, Direction::Left, Direction::Up, Direction::Left, Direction::Right, Direction::Right}),
                                     MapEncoding::Jumper}
{
    _velX = ENEMY_X_VELOCITY;
    _jumper = JUMPER;
}

void CreateJumper(Position position, std::vector<std::shared_ptr<Entity>> &entities)
{
    entities.push_back(std::shared_ptr<Jumper>(new Jumper(position.ix, position.iy)));
}

auto Jumper::Draw() -> void
{
    if (!_lives)
    {
        return;
    }

    glBindTexture(GL_TEXTURE_2D, _jumper->GetTexture());
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);

    glTexCoord2f(0, 0);
    glVertex2f(_x - 1.f, _y - 1.f);

    glTexCoord2f(0, 1);
    glVertex2f(_x - 1.f, _y + DELTA_Y * _sizeY - 1.f);

    glTexCoord2f(1, 1);
    glVertex2f(_x + DELTA_X * _sizeX - 1.f, _y + DELTA_Y * _sizeY - 1.f);

    glTexCoord2f(1, 0);
    glVertex2f(_x + DELTA_X * _sizeX - 1.f, _y - 1.f);

    glEnd();

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

auto Jumper::Run(std::unordered_set<std::shared_ptr<Object>> neighbours) -> void
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

auto Jumper::GetBonus() -> std::optional<std::shared_ptr<Bonus>>
{
    if (std::rand() % 10 <= 1)
    {
        return std::shared_ptr<Bonus>(new Bonus(_x, _y, BonusType::OneLife));
    }

    return std::nullopt;
}
