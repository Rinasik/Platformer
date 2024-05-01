#pragma once

#include <GL/glut.h>
#include <optional>

#include "../entity/entity.hpp"
#include "../platform/platform.hpp"
#include "../exit/exit.hpp"
#include "../hit/hit.hpp"
#include "../../texture/texture.hpp"
#include "../bonus/bonus.hpp"

class Hero : virtual public Entity
{
private:
    int _lives;
    bool _isFalling = false;
    bool _isInvisible = false;
    bool _isDoubleJump = false;
    bool _isDoubleJumped = false;
    int _invisibleCounter = 0;

    int _maxLives = 0;

    std::optional<Hit *> _hit = std::nullopt;
    std::function<void(Hit *)> _addHit;

    Texture _heart;

    auto computeCollision(std::unordered_set<Object *> neighbours, double &virtualDeltaX, double &virtualDeltaY) -> void;
    auto entitiesAndMapCollision(std::unordered_set<Object *> neighbours, double &virtualDeltaX, double &virtualDeltaY) -> void;
    auto entitiesAndMapCollisionY(std::unordered_set<Object *> neighbours, double &virtualDeltaX, double &virtualDeltaY) -> void;

    auto collisionLeftDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void;
    auto collisionRightDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void;
    auto collisionBottomDetected(Object *neighbour, Shape nShape, double &virtualDeltaY) -> void;
    auto collisionTopDetected(Object *neighbour, Shape nShape, double &virtualDeltaY) -> void;

    auto getDamage(Direction direction) -> void;

    auto handleBonus(BonusType type) -> void;

public:
    int level = 0;

    Hero();
    Hero(int ix, int iy, int sizeX, int sizeY, int lives, std::function<void(Hit *)> addHit);

    auto Run(std::unordered_set<Object *> neighbours) -> void;
    auto Draw() -> void;
    auto HandleSpecialClickDown(int key) -> void;
    auto HandleSpecialClickUp(int key) -> void;
    auto HandleClickDown(unsigned char key) -> void;
};

Hero::Hero(){};
Hero::Hero(int ix, int iy, int sizeX, int sizeY, int lives, std::function<void(Hit *)> addHit) : Entity{
                                                                                                     (double)ix,
                                                                                                     (double)iy,
                                                                                                     (double)sizeX,
                                                                                                     (double)sizeY,
                                                                                                     MapEncoding::Hero},
                                                                                                 _lives(lives), _maxLives(lives), _addHit(addHit)
{
    _heart = Texture("images/Heart.png", true);
}

auto Hero::Draw() -> void
{
    // Отрисовка жизней
    for (int i = 0; i < _lives; ++i)
    {
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D, _heart.GetTexture());
        glEnable(GL_TEXTURE_2D);

        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

        glBegin(GL_QUADS);

        glTexCoord2f(0, 1);
        glVertex2f(DELTA_X * (1.2 * i + 0.2) / 2 - 1.f, -DELTA_Y * (0.2) / 2 + 1.f);

        glTexCoord2f(0, 0);
        glVertex2f(DELTA_X * (1.2 * i + 0.2) / 2 - 1.f, -DELTA_Y * (1.2) / 2 + 1.f);

        glTexCoord2f(1, 0);
        glVertex2f(DELTA_X * (1.2 * i + 1.2) / 2 - 1.f, -DELTA_Y * (1.2) / 2 + 1.f);

        glTexCoord2f(1, 1);
        glVertex2f(DELTA_X * (1.2 * i + 1.2) / 2 - 1.f, -DELTA_Y * (0.2) / 2 + 1.f);

        glEnd();

        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    if (!_lives)
    {
        return;
    }

    if (!_isInvisible || _invisibleCounter % 2)
    {
        glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

        glBegin(GL_QUADS);

        glVertex2f(_x - 1.f, _y - 1.f);
        glVertex2f(_x - 1.f, _y + DELTA_Y * _sizeY - 1.f);
        glVertex2f(_x + DELTA_X * _sizeX - 1.f, _y + DELTA_Y * _sizeY - 1.f);
        glVertex2f(_x + DELTA_X * _sizeX - 1.f, _y - 1.f);

        glEnd();
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
}

auto Hero::HandleSpecialClickDown(int key) -> void
{
    if (!_lives)
    {
        return;
    }

    switch (key)
    {
    case GLUT_KEY_RIGHT:
        if (abs(_velX) < MAX_X_VELOCITY)
            _velX += DELTA_X_VELOCITY;
        break;
    case GLUT_KEY_LEFT:
        if (abs(_velX) < MAX_X_VELOCITY)
            _velX -= DELTA_X_VELOCITY;
        break;
    case GLUT_KEY_UP:
        if (!_isFalling || (_isDoubleJump && !_isDoubleJumped))
        {
            if (_isFalling)
            {
                _isDoubleJumped = true;
            }
            _velY += DELTA_Y_VELOCITY;
            _isFalling = true;
        }
        break;
    case GLUT_KEY_DOWN:
        _sizeY = 0.5;
        break;
    default:
        break;
    }
}

auto Hero::HandleSpecialClickUp(int key) -> void
{
    if (!_lives)
    {
        return;
    }

    switch (key)
    {
    case GLUT_KEY_DOWN:
        _sizeY = 1;
        break;
    default:
        break;
    }
}

auto Hero::HandleClickDown(unsigned char key) -> void
{
    if (!_lives)
    {
        return;
    }

    switch (key)
    {
    case 'a':
        if (!_hit.has_value())
        {
            _hit = std::optional(new Hit(_x / DELTA_X, HEIGHT - 1 - _y / DELTA_Y, Direction::Left));
            _addHit(_hit.value());
        }
        break;
    case 'd':
        if (!_hit.has_value())
        {
            _hit = std::optional(new Hit(_x / DELTA_X, HEIGHT - 1 - _y / DELTA_Y, Direction::Right));
            _addHit(_hit.value());
        }
        break;
    default:
        break;
    }
}

auto Hero::handleBonus(BonusType type) -> void
{
    if (type == BonusType::MaxLives)
    {
        _maxLives += 1;
    }
    else if (type == BonusType::OneLife)
    {
        _lives += 1;
        if (_lives > _maxLives)
        {
            _lives = _maxLives;
        }
    }
    else if (type == BonusType::ThreeLives)
    {
        _lives += 3;
        if (_lives > _maxLives)
        {
            _lives = _maxLives;
        }
    }
    else if (type == BonusType::DoubleJump)
    {
        _isDoubleJump = true;
    }
}

auto Hero::Run(std::unordered_set<Object *> neighbours) -> void
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
        return;
    }

    auto initialVelX = _velX;
    auto initialVelY = _velY;

    if (abs(_velX - Sign(_velX) * X_ACC) <= EPSILON)
    {
        _velX = 0;
    }
    else
    {
        _velX -= Sign(_velX) * X_ACC;
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

    auto virtualDeltaX = (_velX + initialVelX) / 2;
    auto virtualDeltaY = (_velY + initialVelY) / 20;

    computeCollision(neighbours, virtualDeltaX, virtualDeltaY);

    _x += virtualDeltaX;
    _y += virtualDeltaY;
}

auto Hero::entitiesAndMapCollisionY(std::unordered_set<Object *> neighbours, double &virtualDeltaX, double &virtualDeltaY) -> void
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
            if (virtualDeltaY <= 0 && nShape.top <= (shape.bottom) && nShape.top >= (shape.bottom + virtualDeltaY))
            {
                collisionBottomDetected(neighbour, nShape, virtualDeltaY);
            }
            // Летит вверх
            else if (virtualDeltaY >= 0 && (nShape.bottom >= (shape.top)) && (nShape.bottom <= (shape.top + virtualDeltaY)))
            {
                collisionTopDetected(neighbour, nShape, virtualDeltaY);
            }
        }
        auto isBelowThanTop = abs(nShape.bottom - shape.top) <= EPSILON;
        // На платформе(не за платформой && не падает && не под платформой)
        if (!(isEntireLeft || isEntireRight) && !_isFalling && !isBelowThanTop)
        {
            onPlatform = true;
        }
    }

    if (_y > EPSILON)
    {
        _isFalling = !onPlatform;
    }

    if (shape.bottom + virtualDeltaY <= 0)
    {
        _isFalling = false;
        _isDoubleJumped = false;
    }
}

auto Hero::entitiesAndMapCollision(std::unordered_set<Object *> neighbours, double &virtualDeltaX, double &virtualDeltaY) -> void
{
    objectsCollisionX(neighbours, virtualDeltaX);
    entitiesAndMapCollisionY(neighbours, virtualDeltaX, virtualDeltaY);
}

auto Hero::computeCollision(std::unordered_set<Object *> neighbours, double &virtualDeltaX, double &virtualDeltaY) -> void
{
    windowBoundsCollision(virtualDeltaX, virtualDeltaY);
    entitiesAndMapCollision(neighbours, virtualDeltaX, virtualDeltaY);
}

auto Hero::collisionRightDetected(Object *neighbour, Shape nShape, double &virtualDeltaX) -> void
{
    if (neighbour->type == MapEncoding::Brick || neighbour->type == MapEncoding::Platform)
    {
        _x = nShape.left - _sizeX * DELTA_X;

        _velX = 0;
        virtualDeltaX = 0;
    }
    else if (neighbour->type == MapEncoding::Exit)
    {
        Exit *exit = dynamic_cast<Exit *>(neighbour);
        level = exit->mapNumber;
    }
    else if (neighbour->type == MapEncoding::Magma)
    {
        _lives = 0;
    }
    else if (neighbour->type == MapEncoding::Bonus)
    {
        Bonus *bonus = dynamic_cast<Bonus *>(neighbour);
        handleBonus(bonus->type);

        bonus->isDestroyed = true;
    }
    else if (!_isInvisible && (neighbour->type == MapEncoding::Warrior || neighbour->type == MapEncoding::Jumper || neighbour->type == MapEncoding::Archer || neighbour->type == MapEncoding::Monster))
    {
        getDamage(Direction::Left);
    }
}

auto Hero::collisionLeftDetected(Object *neighbour, Shape nShape, double &virtualDeltaX) -> void
{
    if (neighbour->type == MapEncoding::Brick || neighbour->type == MapEncoding::Platform)
    {
        _x = nShape.right;

        _velX = 0;
        virtualDeltaX = 0;
    }
    else if (neighbour->type == MapEncoding::Exit)
    {
        Exit *exit = dynamic_cast<Exit *>(neighbour);
        level = exit->mapNumber;
    }
    else if (neighbour->type == MapEncoding::Magma)
    {
        _lives = 0;
    }
    else if (neighbour->type == MapEncoding::Bonus)
    {
        Bonus *bonus = dynamic_cast<Bonus *>(neighbour);
        handleBonus(bonus->type);

        bonus->isDestroyed = true;
    }
    else if (!_isInvisible && (neighbour->type == MapEncoding::Warrior || neighbour->type == MapEncoding::Jumper || neighbour->type == MapEncoding::Archer || neighbour->type == MapEncoding::Monster))
    {
        getDamage(Direction::Right);
    }
}

auto Hero::collisionBottomDetected(Object *neighbour, Shape nShape, double &virtualDeltaY) -> void
{
    if (neighbour->type == MapEncoding::Brick)
    {
        _y = nShape.top;

        _isFalling = false;
        _isDoubleJumped = false;
        _velY = 0;

        virtualDeltaY = 0;
    }
    else if (neighbour->type == MapEncoding::Exit)
    {
        Exit *exit = dynamic_cast<Exit *>(neighbour);
        level = exit->mapNumber;
    }
    else if (neighbour->type == MapEncoding::Platform)
    {
        Platform *platform = dynamic_cast<Platform *>(neighbour);
        _x += platform->lastDeltaX;
        _y = nShape.top;

        _isFalling = false;
        _isDoubleJumped = false;
        _velY = 0;

        virtualDeltaY = 0;
    }
    else if (neighbour->type == MapEncoding::Magma)
    {
        _lives = 0;
    }
    else if (neighbour->type == MapEncoding::Bonus)
    {
        Bonus *bonus = dynamic_cast<Bonus *>(neighbour);
        handleBonus(bonus->type);

        bonus->isDestroyed = true;
    }
    else if (!_isInvisible && (neighbour->type == MapEncoding::Warrior || neighbour->type == MapEncoding::Jumper || neighbour->type == MapEncoding::Archer || neighbour->type == MapEncoding::Monster))
    {
        getDamage(Direction::Up);
    }
}

auto Hero::collisionTopDetected(Object *neighbour, Shape nShape, double &virtualDeltaY) -> void
{
    if (neighbour->type == MapEncoding::Brick || neighbour->type == MapEncoding::Platform)
    {
        _y = nShape.bottom - _sizeY * DELTA_Y;

        _velY = -_velY / 2;
        virtualDeltaY = 0;
    }
    else if (neighbour->type == MapEncoding::Exit)
    {
        Exit *exit = dynamic_cast<Exit *>(neighbour);
        level = exit->mapNumber;
    }
    else if (neighbour->type == MapEncoding::Magma)
    {
        _lives = 0;
    }
    else if (neighbour->type == MapEncoding::Bonus)
    {
        Bonus *bonus = dynamic_cast<Bonus *>(neighbour);
        handleBonus(bonus->type);

        bonus->isDestroyed = true;
    }
}

// Direction - направление куда отскачило
auto Hero::getDamage(Direction direction) -> void
{
    _velY = 0.4;
    _lives -= 1;
    _isInvisible = true;

    if (direction == Direction::Left)
    {
        _velX = -0.008f;
    }
    else if (direction == Direction::Right)
    {
        _velX = 0.008f;
    }
}