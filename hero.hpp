#pragma once

#include "entity.hpp"
#include <GL/glut.h>

class Hero : virtual public Entity
{
private:
    int _lives;
    bool _alive = true;
    bool _isFalling = false;

    auto computeCollision(std::vector<Entity *> neighbours, double &virtualDeltaX, double &virtualDeltaY) -> void;
    auto entitiesAndMapCollision(std::vector<Entity *> neighbours, double &virtualDeltaX, double &virtualDeltaY) -> void;

    auto collisionLeftDetected(Entity *neighbour, Shape nshape, double &virtualDeltaX) -> void;
    auto collisionRightDetected(Entity *neighbour, Shape nshape, double &virtualDeltaX) -> void;
    auto collisionBottomDetected(Entity *neighbour, Shape nShape, double &virtualDeltaY) -> void;
    auto collisionTopDetected(Entity *neighbour, Shape nShape, double &virtualDeltaY) -> void;

public:
    int level = 0;

    Hero();
    Hero(int ix, int iy, int sizeX, int sizeY, int lives);

    auto Run(std::vector<Entity *> neighbours) -> void;
    auto Draw() -> void;
    auto HandleClick(int key) -> void;
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
}

auto Hero::Draw() -> void
{
    if (!_alive)
    {
        return;
    }
    glColor3f(0.0f, 0.0f, 1.0f);

    glBegin(GL_QUADS);

    glVertex2f(_x - 1.f, _y - 1.f);
    glVertex2f(_x - 1.f, _y + DELTA_Y * _sizeY - 1.f);
    glVertex2f(_x + DELTA_X * _sizeX - 1.f, _y + DELTA_Y * _sizeY - 1.f);
    glVertex2f(_x + DELTA_X * _sizeX - 1.f, _y - 1.f);

    glEnd();
}

auto Hero::HandleClick(int key) -> void
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
    default:
        break;
    }
}

auto Hero::Run(std::vector<Entity *> neighbours) -> void
{
    if (!_alive)
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

auto Hero::entitiesAndMapCollision(std::vector<Entity *> neighbours, double &virtualDeltaX, double &virtualDeltaY) -> void
{
    auto shape = GetShape();
    auto onPlatform = false;

    entitiesAndMapCollisionX(neighbours, virtualDeltaX, virtualDeltaY);

    for (auto neighbour : neighbours)
    {
        auto nShape = neighbour->GetShape();

        // Летит
        auto isEntireLeft = ((nShape.left - (shape.right + virtualDeltaX)) >= -250 * EPSILON) && ((nShape.left - (shape.left + virtualDeltaX)) >= -250 * EPSILON);
        auto isEntireRight = ((shape.left + virtualDeltaX - nShape.right) >= -250 * EPSILON) && ((shape.right + virtualDeltaX - nShape.right) >= -250 * EPSILON);

        if (_isFalling && !(isEntireLeft || isEntireRight))
        {
            // Летит вниз
            if (virtualDeltaY < 0 && nShape.top <= (shape.bottom) && nShape.top >= (shape.bottom + virtualDeltaY))
            {
                collisionTopDetected(neighbour, nShape, virtualDeltaY);
            }
            // Летит вверх
            else if (virtualDeltaY > 0 && (nShape.bottom >= (shape.top)) && (nShape.bottom <= (shape.top + virtualDeltaY)))
            {
                collisionBottomDetected(neighbour, nShape, virtualDeltaY);
            }
        }
        // На платформе платформы
        if (!(isEntireLeft || isEntireRight) && !_isFalling)
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

auto Hero::computeCollision(std::vector<Entity *> neighbours, double &virtualDeltaX, double &virtualDeltaY) -> void
{
    windowBoundsCollision(virtualDeltaX, virtualDeltaY);
    entitiesAndMapCollision(neighbours, virtualDeltaX, virtualDeltaY);
}

auto Hero::collisionRightDetected(Entity *neighbour, Shape nShape, double &virtualDeltaX) -> void
{
    if (neighbour->type == MapEncoding::Brick)
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
}

auto Hero::collisionLeftDetected(Entity *neighbour, Shape nShape, double &virtualDeltaX) -> void
{
    if (neighbour->type == MapEncoding::Brick)
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
}

auto Hero::collisionTopDetected(Entity *neighbour, Shape nShape, double &virtualDeltaY) -> void
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
}

auto Hero::collisionBottomDetected(Entity *neighbour, Shape nShape, double &virtualDeltaY) -> void
{
    if (neighbour->type == MapEncoding::Brick)
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
}