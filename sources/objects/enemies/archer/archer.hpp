#pragma once

#include "../enemy/enemy.hpp"
#include "../../weapon/arrow/arrow.hpp"

class Archer : virtual public Enemy
{
private:
    std::optional<Arrow *> _arrow = std::nullopt;
    std::function<void(Arrow *)> _addArrow;

public:
    Archer(int ix, int iy, std::function<void(Arrow *)> addArrow);

    auto Run(std::unordered_set<Object *> neighbours) -> void;
    auto Draw() -> void;

    auto GetBonus() -> std::optional<Bonus *>;
};

Archer::Archer(int ix, int iy, std::function<void(Arrow *)> addArrow) : Enemy{
                                                                            (double)ix,
                                                                            (double)iy,
                                                                            1, 1, 2,
                                                                            std::vector<Direction>(),
                                                                            MapEncoding::Archer},
                                                                        _addArrow(addArrow)
{
    _velX = 0;
}

void CreateArcher(Position position, std::unordered_set<Entity *> &entities, std::function<void(Arrow *)> addArrow)
{
    entities.emplace(new Archer(position.ix, position.iy, addArrow));
}

auto Archer::Draw() -> void
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

auto Archer::Run(std::unordered_set<Object *> neighbours) -> void
{
    if (_arrow.has_value())
    {
        if (_arrow.value()->isDestroyed)
        {
            _arrow = std::nullopt;
        }
    }

    if (!_lives)
    {
        isDestroyed = true;
        return;
    }

    if (!_arrow.has_value())
    {
        if (std::rand() % 2)
        {
            _arrow = std::optional(new Arrow(_x / DELTA_X, HEIGHT - 1 - _y / DELTA_Y, Direction::Left, this));
        }
        else
        {
            _arrow = std::optional(new Arrow(_x / DELTA_X, HEIGHT - 1 - _y / DELTA_Y, Direction::Right, this));
        }
        _addArrow(_arrow.value());
    }

    double virtualDeltaX = 0;

    objectsCollisionX(neighbours, virtualDeltaX);
}

auto Archer::GetBonus() -> std::optional<Bonus *>
{
    int probability = std::rand() % 10;

    if (probability <= 1)
    {
        return new Bonus(_x, _y, BonusType::OneLife);
    }
    else if (probability >= 2 && probability <= 4)
    {
        return new Bonus(_x, _y, BonusType::Arrows);
    }

    return std::nullopt;
}