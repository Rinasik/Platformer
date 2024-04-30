#pragma once

#include <GL/glut.h>
#include "../entity/entity.hpp"

class Enemy : virtual public Entity
{
private:
    double _velX = ENEMY_X_VELOCITY;
    int _currentStep = 0;
    double _prevX = 0;
    double _velY = 0;

    std::vector<Direction> _pattern;

    int _lives;

    auto mapCollision(std::set<MapEncoding> mapPattern) -> void;

    auto collisionLeftDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void;
    auto collisionRightDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void;

public:
    Enemy(int ix, int iy, int sizeX, int sizeY, int lives);

    auto Run(std::set<Object *> neighbours) -> void;
    auto Draw() -> void;
};

void CreateEnemie(Position position, std::set<Entity *> &entities)
{
    entities.emplace(new Enemy(position.ix, position.iy, 1, 1, 2));
}

Enemy::Enemy(int ix, int iy, int sizeX, int sizeY, int lives) : Entity{
                                                                    (double)ix,
                                                                    (double)iy,
                                                                    (double)sizeX,
                                                                    (double)sizeY,
                                                                    MapEncoding::Enemy},
                                                                _lives(lives)
{
    _pattern = std::vector<Direction>({Direction::Left, Direction::Left, Direction::Right, Direction::Right, Direction::Right, Direction::Right, Direction::Left, Direction::Left});
    _prevX = _x;
}

auto Enemy::Draw() -> void
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

auto Enemy::Run(std::set<Object *> neighbours) -> void
{
    if (!_lives)
    {
        return;
    }

    float currentPos = abs((_x - _prevX) / DELTA_X);

    if (currentPos >= 1)
    {
        int _nextStep = (_currentStep + 1) % _pattern.size();
        if (_pattern[_currentStep] != _pattern[_nextStep])
        {
            _velX = -_velX;
        }

        _prevX = _x;
        _currentStep = _nextStep;
    }

    _x += _velX;
}

auto Enemy::mapCollision(std::set<MapEncoding> mapPattern) -> void
{
}

auto Enemy::collisionLeftDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void {}
auto Enemy::collisionRightDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void {}