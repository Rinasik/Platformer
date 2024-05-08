#pragma once

#include "../enemy/enemy.hpp"

enum class BossPhase
{
    First = 1,
    Second = 2
};

class Boss : virtual public Enemy
{
private:
    bool _isFalling = false;

    int _prevLives;
    int _invisibleCounter = 0;

    BossPhase _phase;

    auto entitiesAndMapCollisionY(std::unordered_set<std::shared_ptr<Object>> neighbours, double &virtualDeltaX, double &virtualDeltaY) -> void;

    auto collisionBottomDetected(std::shared_ptr<Object> neighbour, Shape nShape, double &virtualDeltaY) -> void;
    auto collisionTopDetected(std::shared_ptr<Object> neighbour, Shape nShape, double &virtualDeltaY) -> void;
    auto collisionLeftDetected(std::shared_ptr<Object> neighbour, Shape nshape, double &virtualDeltaX) -> void;
    auto collisionRightDetected(std::shared_ptr<Object> neighbour, Shape nshape, double &virtualDeltaX) -> void;

public:
    Boss(int ix, int iy);

    auto Run(std::unordered_set<std::shared_ptr<Object>> neighbours) -> void;
    auto Draw() -> void;

    auto GetBonus() -> std::optional<std::shared_ptr<Bonus>>;
};

Boss::Boss(int ix, int iy) : Enemy{
                                 (double)ix,
                                 (double)iy,
                                 2, 2, 10,
                                 std::vector<Direction>({Direction::Right, Direction::Up, Direction::Right, Direction::Left, Direction::Left, Direction::Left, Direction::Up, Direction::Left, Direction::Right, Direction::Right}),
                                 MapEncoding::Boss},
                             _phase(BossPhase::First)
{
    _velX = 0.8 * MAX_X_VELOCITY;
    _prevLives = 10;
}

auto Boss::Run(std::unordered_set<std::shared_ptr<Object>> neighbours) -> void
{
    if (!_lives)
    {
        isDestroyed = true;
        return;
    }

    else if (_lives != _prevLives)
    {
        _isInvisible = true;
        _prevLives = _lives;

        if (_lives == 5)
        {
            _phase = BossPhase::Second;
        }
    }

    if (_isInvisible)
    {
        _invisibleCounter++;
        if (_invisibleCounter == INVISIBLE_TICKS_COUNT)
        {
            _invisibleCounter = 0;
            _isInvisible = false;
        }
    }

    auto initialVelY = _velY;
    double virtualDeltaX = 0;

    for (auto &neighbour : neighbours)
    {
        if (neighbour->type == MapEncoding::Hero)
        {
            Shape shape = GetShape();
            Shape nShape = neighbour->GetShape();

            auto isAboveThanBottom = nShape.top - shape.bottom >= -DELTA_Y;
            auto isBelowThanTop = (nShape.bottom - shape.top) <= DELTA_Y;

            if (isAboveThanBottom && isBelowThanTop)
            {
                if (shape.right - nShape.right >= 0)
                {
                    virtualDeltaX = -_velX;
                }
                else if (shape.left - nShape.left <= 0)
                {
                    virtualDeltaX = _velX;
                }

                if (_phase == BossPhase::Second)
                {
                    double distance = abs(std::min(shape.left - nShape.left, shape.right - nShape.right));
                    if (nShape.bottom - shape.bottom >= 0 && distance <= 2 * DELTA_X && !_isFalling)
                    {
                        _velY += DELTA_Y_VELOCITY;
                        _isFalling = true;
                    }
                }
            }
        }
    }

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

    objectsCollisionX(neighbours, virtualDeltaX);
    entitiesAndMapCollisionY(neighbours, virtualDeltaX, virtualDeltaY);

    _x += virtualDeltaX;
    _y += virtualDeltaY;
}

auto Boss::entitiesAndMapCollisionY(std::unordered_set<std::shared_ptr<Object>> neighbours, double &virtualDeltaX, double &virtualDeltaY) -> void
{
    auto shape = GetShape();
    auto onPlatform = false;

    for (auto neighbour : neighbours)
    {
        auto nShape = neighbour->GetShape();

        // Вне платформы
        auto isEntireLeft = ((nShape.left - (shape.right + virtualDeltaX)) >= -250 * EPSILON) && ((nShape.left - (shape.left + virtualDeltaX)) >= -250 * EPSILON);
        auto isEntireRight = ((shape.left + virtualDeltaX - nShape.right) >= -250 * EPSILON) && ((shape.right + virtualDeltaX - nShape.right) >= -250 * EPSILON);

        if (!(isEntireLeft || isEntireRight))
        {
            // Летит вниз
            if (virtualDeltaY <= 0 && (nShape.top - (shape.bottom)) <= EPSILON && nShape.top >= (shape.bottom + virtualDeltaY))
            {
                collisionBottomDetected(neighbour, nShape, virtualDeltaY);
            }
            // Летит вверх
            if (virtualDeltaY >= 0 && (nShape.bottom >= (shape.top)) && (nShape.bottom <= (shape.top + virtualDeltaY)))
            {
                collisionTopDetected(neighbour, nShape, virtualDeltaY);
            }

            auto isOnTop = abs(nShape.top - shape.bottom) <= EPSILON;
            // На платформе(не за платформой && не падает && не под платформой)
            if (!_isFalling && isOnTop)
            {
                onPlatform = true;
            }
        }
    }

    if (_y > EPSILON)
    {
        _isFalling = !onPlatform;
    }

    if (shape.bottom + virtualDeltaY <= 0)
    {
        _isFalling = false;
    }
}

auto Boss::collisionBottomDetected(std::shared_ptr<Object> neighbour, Shape nShape, double &virtualDeltaY) -> void
{
    if (neighbour->type == MapEncoding::Brick || neighbour->type == MapEncoding::Chest || neighbour->type == MapEncoding::Box)
    {
        _y = nShape.top;

        _isFalling = false;
        _velY = 0;

        virtualDeltaY = 0;
    }
    else if (neighbour->type == MapEncoding::Platform)
    {
        auto platform = std::dynamic_pointer_cast<Platform>(neighbour);
        _x += platform->lastDeltaX;
        _y = nShape.top;

        _isFalling = false;
        _velY = 0;

        virtualDeltaY = 0;
    }
    else if (neighbour->type == MapEncoding::Magma)
    {
        _lives = 0;
    }
    else if (!_isInvisible && (neighbour->type == MapEncoding::Arrow || neighbour->type == MapEncoding::Hit))
    {
        auto munition = std::dynamic_pointer_cast<Munition>(neighbour);
        if (this != munition->owner)
        {
            _lives--;
            munition->isDestroyed = true;
        }
    }
}

auto Boss::collisionTopDetected(std::shared_ptr<Object> neighbour, Shape nShape, double &virtualDeltaY) -> void
{
    if (neighbour->type == MapEncoding::Brick || neighbour->type == MapEncoding::Platform || neighbour->type == MapEncoding::Chest || neighbour->type == MapEncoding::Box)
    {
        _y = nShape.bottom - _sizeY * DELTA_Y;

        _velY = -_velY / 2;
        virtualDeltaY = 0;
    }
    else if (neighbour->type == MapEncoding::Magma)
    {
        _lives = 0;
    }
    else if (!_isInvisible && (neighbour->type == MapEncoding::Arrow || neighbour->type == MapEncoding::Hit))
    {
        auto munition = std::dynamic_pointer_cast<Munition>(neighbour);
        if (this != munition->owner)
        {
            _lives--;
            munition->isDestroyed = true;
        }
    }
}

auto Boss::collisionLeftDetected(std::shared_ptr<Object> neighbour, Shape nshape, double &virtualDeltaX) -> void
{
    if (neighbour->type == MapEncoding::Brick || neighbour->type == MapEncoding::Platform || neighbour->type == MapEncoding::Chest || neighbour->type == MapEncoding::Box)
    {
        _x = nshape.right;

        virtualDeltaX = 0;
    }
    else if (!_isInvisible && (neighbour->type == MapEncoding::Arrow || neighbour->type == MapEncoding::Hit))
    {
        auto munition = std::dynamic_pointer_cast<Munition>(neighbour);
        if (this != munition->owner)
        {
            _lives--;
            munition->isDestroyed = true;
        }
    }
}

auto Boss::collisionRightDetected(std::shared_ptr<Object> neighbour, Shape nshape, double &virtualDeltaX) -> void
{
    if (neighbour->type == MapEncoding::Brick || neighbour->type == MapEncoding::Platform || neighbour->type == MapEncoding::Chest || neighbour->type == MapEncoding::Box)
    {
        _x = nshape.left - _sizeX * DELTA_X;

        virtualDeltaX = 0;
    }
    else if (!_isInvisible && (neighbour->type == MapEncoding::Arrow || neighbour->type == MapEncoding::Hit))
    {
        auto munition = std::dynamic_pointer_cast<Munition>(neighbour);
        if (this != munition->owner)
        {
            _lives--;
            munition->isDestroyed = true;
        }
    }
}

auto Boss::GetBonus() -> std::optional<std::shared_ptr<Bonus>>
{
    return std::nullopt;
}

auto Boss::Draw() -> void
{
    if (!_isInvisible || _invisibleCounter % 2)
    {
        glColor4f(0.87f, 0.0f, 0.32f, 1.0f);

        glBegin(GL_QUADS);

        glVertex2f(_x - 1.f, _y - 1.f);
        glVertex2f(_x - 1.f, _y + DELTA_Y * _sizeY - 1.f);
        glVertex2f(_x + DELTA_X * _sizeX - 1.f, _y + DELTA_Y * _sizeY - 1.f);
        glVertex2f(_x + DELTA_X * _sizeX - 1.f, _y - 1.f);

        glEnd();
    }
}
