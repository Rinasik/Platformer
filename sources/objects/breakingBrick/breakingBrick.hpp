#pragma once

#include <GL/glut.h>
#include "../entity/entity.hpp"

class BreakingBrick : virtual public Entity
{
private:
    Texture *_brick;
    int _lives = 0;

    auto collisionLeftDetected(std::shared_ptr<Object> neighbour, Shape nshape, double &virtualDeltaX) -> void;
    auto collisionRightDetected(std::shared_ptr<Object> neighbour, Shape nshape, double &virtualDeltaX) -> void;

public:
    BreakingBrick(double ix, double iy, int lives);

    auto Draw() -> void;
    auto Run(std::unordered_set<std::shared_ptr<Object>> neighbours) -> void;

    auto GetBonus() -> std::optional<std::shared_ptr<Bonus>>;
};

BreakingBrick::BreakingBrick(double ix, double iy, int lives) : Entity{
                                                                    ix,
                                                                    iy,
                                                                    1,
                                                                    1,
                                                                    MapEncoding::BreakingBrick}
{
    _brick = WALL;
    _lives = lives;
};

auto CreateBreakingBrick(Position position, std::vector<std::shared_ptr<Entity>> &entities) -> void
{
    entities.push_back(std::shared_ptr<BreakingBrick>(new BreakingBrick(position.ix, position.iy, 3)));
}

auto BreakingBrick::Draw() -> void
{
    glBindTexture(GL_TEXTURE_2D, _brick->GetTexture());
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);

    glTexCoord2f(0, 0);
    glVertex2f(_x - 1.0f, _y - 1.0f);

    glTexCoord2f(0, 1);
    glVertex2f(_x - 1.0f, _y + _sizeY * DELTA_Y - 1.0f);

    glTexCoord2f(1, 1);
    glVertex2f(_x + _sizeX * DELTA_X - 1.0f, _y + _sizeY * DELTA_Y - 1.0f);

    glTexCoord2f(1, 0);
    glVertex2f(_x + _sizeX * DELTA_X - 1.0f, _y - 1.0f);

    glEnd();

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
};

auto BreakingBrick::GetBonus() -> std::optional<std::shared_ptr<Bonus>>
{
    return std::nullopt;
}

auto BreakingBrick::Run(std::unordered_set<std::shared_ptr<Object>> neighbours) -> void
{
    if (!_lives)
    {
        isDestroyed = true;

        return;
    }

    double virtualDeltaX = 0;
    objectsCollisionX(neighbours, virtualDeltaX);
};

auto BreakingBrick::collisionLeftDetected(std::shared_ptr<Object> neighbour, Shape nshape, double &virtualDeltaX) -> void
{
    if (neighbour->type == MapEncoding::Hit)
    {
        auto hit = std::dynamic_pointer_cast<Hit>(neighbour);

        hit->isDestroyed = true;
        _lives--;
    }
};
auto BreakingBrick::collisionRightDetected(std::shared_ptr<Object> neighbour, Shape nshape, double &virtualDeltaX) -> void
{
    if (neighbour->type == MapEncoding::Hit)
    {
        auto hit = std::dynamic_pointer_cast<Hit>(neighbour);

        hit->isDestroyed = true;
        _lives--;
    }
};