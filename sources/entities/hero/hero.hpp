#pragma once

#include <GL/glut.h>

#include "../entity/entity.hpp"
#include "../platform/platform.hpp"
#include "../exit/exit.hpp"
#include "../../texture/texture.hpp"

class Hero : virtual public Entity
{
private:
    int _lives;
    bool _isFalling = false;
    bool _isInvisible = false;
    int _invisibleCounter = 0;

    Texture _heart;

    auto computeCollision(std::set<Object *> neighbours, double &virtualDeltaX, double &virtualDeltaY) -> void;
    auto entitiesAndMapCollision(std::set<Object *> neighbours, double &virtualDeltaX, double &virtualDeltaY) -> void;
    auto entitiesAndMapCollisionY(std::set<Object *> neighbours, double &virtualDeltaX, double &virtualDeltaY) -> void;

    auto collisionLeftDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void;
    auto collisionRightDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void;
    auto collisionBottomDetected(Object *neighbour, Shape nShape, double &virtualDeltaY) -> void;
    auto collisionTopDetected(Object *neighbour, Shape nShape, double &virtualDeltaY) -> void;

    auto getDamage(Direction direction) -> void;

public:
    int level = 0;

    Hero();
    Hero(int ix, int iy, int sizeX, int sizeY, int lives);

    auto Run(std::set<Object *> neighbours) -> void;
    auto Draw() -> void;
    auto HandleClickDown(int key) -> void;
    auto HandleClickUp(int key) -> void;
};

Hero::Hero(){};
Hero::Hero(int ix, int iy, int sizeX, int sizeY, int lives) : Entity{
                                                                  ix,
                                                                  iy,
                                                                  sizeX,
                                                                  sizeY,
                                                                  MapEncoding::Hero},
                                                              _lives(lives)
{
    _heart = Texture("images/Heart.png", true);
}

// auto Hero::Draw(unsigned int textId = 0) -> void
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
        glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

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

auto Hero::HandleClickDown(int key) -> void
{
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
        if (!_isFalling)
        {
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

auto Hero::HandleClickUp(int key) -> void
{
    switch (key)
    {
    case GLUT_KEY_DOWN:
        _sizeY = 1;
        break;
    default:
        break;
    }
}

auto Hero::Run(std::set<Object *> neighbours) -> void
{
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

auto Hero::entitiesAndMapCollisionY(std::set<Object *> neighbours, double &virtualDeltaX, double &virtualDeltaY) -> void
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
    }
}

auto Hero::entitiesAndMapCollision(std::set<Object *> neighbours, double &virtualDeltaX, double &virtualDeltaY) -> void
{
    objectsCollisionX(neighbours, virtualDeltaX);
    entitiesAndMapCollisionY(neighbours, virtualDeltaX, virtualDeltaY);
}

auto Hero::computeCollision(std::set<Object *> neighbours, double &virtualDeltaX, double &virtualDeltaY) -> void
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
    else if (!_isInvisible && neighbour->type == MapEncoding::Enemy)
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
    else if (!_isInvisible && neighbour->type == MapEncoding::Enemy)
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
        _velY = 0;

        virtualDeltaY = 0;
    }
    else if (neighbour->type == MapEncoding::Magma)
    {
        _lives = 0;
    }
    else if (!_isInvisible && neighbour->type == MapEncoding::Enemy)
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