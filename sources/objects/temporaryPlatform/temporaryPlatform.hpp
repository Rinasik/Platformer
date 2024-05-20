#pragma once

#include <GL/glut.h>
#include "../entity/entity.hpp"

class TemporaryPLatform : virtual public Entity
{
private:
    Texture *_platform;
    int _ticks;

    auto collisionLeftDetected(std::shared_ptr<Object> neighbour, Shape nshape, double &virtualDeltaX) -> void;
    auto collisionRightDetected(std::shared_ptr<Object> neighbour, Shape nshape, double &virtualDeltaX) -> void;

public:
    TemporaryPLatform(double ix, double iy);
    bool isInvisible = false;

    auto Draw() -> void;
    auto Run(std::unordered_set<std::shared_ptr<Object>> neighbours) -> void;

    auto GetBonus() -> std::optional<std::shared_ptr<Bonus>>;
};

TemporaryPLatform::TemporaryPLatform(double ix, double iy) : Entity{
                                                                 ix,
                                                                 iy,
                                                                 1,
                                                                 1,
                                                                 MapEncoding::TemporaryPlatform}
{
    _platform = WALL;
};

auto CreateTemporaryPlatform(Position position, std::vector<std::shared_ptr<Entity>> &entities) -> void
{
    entities.push_back(std::shared_ptr<TemporaryPLatform>(new TemporaryPLatform(position.ix, position.iy)));
}

auto TemporaryPLatform::Draw() -> void
{
    if (isInvisible)
    {
        return;
    }

    glBindTexture(GL_TEXTURE_2D, _platform->GetTexture());
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

auto TemporaryPLatform::GetBonus() -> std::optional<std::shared_ptr<Bonus>>
{
    return std::nullopt;
}

auto TemporaryPLatform::Run(std::unordered_set<std::shared_ptr<Object>> neighbours) -> void
{
    _ticks = (_ticks + 1) % PLATFORM_PERIOD;
    if (_ticks == 0)
    {
        isInvisible = !isInvisible;
    }
};

auto TemporaryPLatform::collisionLeftDetected(std::shared_ptr<Object> neighbour, Shape nshape, double &virtualDeltaX) -> void {};
auto TemporaryPLatform::collisionRightDetected(std::shared_ptr<Object> neighbour, Shape nshape, double &virtualDeltaX) -> void {};