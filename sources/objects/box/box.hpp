#pragma once

#include <GL/glut.h>
#include "../entity/entity.hpp"

class Box : virtual public Entity
{
private:
    Texture *_box;

    auto collisionLeftDetected(std::shared_ptr<Object> neighbour, Shape nshape, double &virtualDeltaX) -> void;
    auto collisionRightDetected(std::shared_ptr<Object> neighbour, Shape nshape, double &virtualDeltaX) -> void;

public:
    Box(double ix, double iy);

    auto Draw() -> void;
    auto Run(std::unordered_set<std::shared_ptr<Object>> neighbours) -> void;

    auto GetBonus() -> std::optional<std::shared_ptr<Bonus>>;
};

Box::Box(double ix, double iy) : Entity{
                                     ix,
                                     iy,
                                     1,
                                     1,
                                     MapEncoding::Box}
{
    _box = BOX;
};

auto CreateBox(Position position, std::unordered_set<std::shared_ptr<Entity>> &entities) -> void
{
    entities.emplace(std::shared_ptr<Box>(new Box(position.ix, position.iy)));
}

auto Box::Draw() -> void
{
    glBindTexture(GL_TEXTURE_2D, _box->GetTexture());
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

auto Box::GetBonus() -> std::optional<std::shared_ptr<Bonus>>
{
    int probability = std::rand() % 10;

    if (probability <= 2)
    {
        return std::shared_ptr<Bonus>(new Bonus(_x, _y, BonusType::OneLife));
    }
    else if (probability >= 3 && probability <= 4)
    {
        return std::shared_ptr<Bonus>(new Bonus(_x, _y, BonusType::ThreeLives));
    }
    else if (probability >= 5 && probability <= 7)
    {
        return std::shared_ptr<Bonus>(new Bonus(_x, _y, BonusType::Arrows));
    }
    else if (probability == 8)
    {
        return std::shared_ptr<Bonus>(new Bonus(_x, _y, BonusType::MaxLives));
    }

    return std::nullopt;
}

auto Box::Run(std::unordered_set<std::shared_ptr<Object>> neighbours) -> void
{
    double virtualDeltaX = 0;
    objectsCollisionX(neighbours, virtualDeltaX);
};

auto Box::collisionLeftDetected(std::shared_ptr<Object> neighbour, Shape nshape, double &virtualDeltaX) -> void
{
    if (neighbour->type == MapEncoding::Hit)
    {
        isDestroyed = true;
    }
};
auto Box::collisionRightDetected(std::shared_ptr<Object> neighbour, Shape nshape, double &virtualDeltaX) -> void
{
    if (neighbour->type == MapEncoding::Hit)
    {
        isDestroyed = true;
    }
};