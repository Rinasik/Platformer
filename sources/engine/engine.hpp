#pragma once

#include <vector>
#include <string>

#include "../map/map.hpp"
#include "../machine/machine.hpp"
#include "../entities/hero/hero.hpp"
#include "../entities/entity/entity.hpp"
#include "../entities/enemy/enemy.hpp"
#include "../entities/platform/platform.hpp"

class Engine
{
private:
    Map _map;
    Machine _machine;

    auto createEntities(std::vector<EntityPosition> positions, std::vector<Entity *> &entities) -> void;

public:
    Engine(std::string path) : _map(Map(WIDTH, HEIGHT, path)), _machine(Machine()){};

    auto InitState(Hero &hero, std::vector<Entity *> &entities) -> void;
    auto UpdateState(Hero &hero, std::vector<Entity *> &entities) -> void;
    auto Draw(Hero &hero, std::vector<Entity *> &entities) -> void;
};

auto Engine::createEntities(std::vector<EntityPosition> positions, std::vector<Entity *> &entities) -> void
{
    for (auto position : positions)
    {
        if (position.entityType == MapEncoding::Enemy)
        {
            CreateEnemie(position.position, entities);
        }
        else if (position.entityType == MapEncoding::Platform)
        {
            CreatePlatforms(position.position, entities);
        }
    }
}

auto Engine::UpdateState(Hero &hero, std::vector<Entity *> &entities) -> void
{
    for (auto entity : entities)
    {
        entity->Run(_machine.FindNearby(entity->GetShape()));
        _machine.UpdatePosition(entity);
    }
    hero.Run(_machine.FindNearby(hero.GetShape()));

    auto opt_pattern = _map.InitDraw(hero.level);
    if (opt_pattern.has_value())
    {
        auto pattern = opt_pattern.value();
        _machine.Clear();

        entities = std::vector<Entity *>();

        createEntities(pattern.positions, entities);

        for (auto brick : pattern.bricks)
        {
            _machine.AddObject(brick);
        }
        for (auto entity : entities)
        {
            _machine.AddObject(entity);
        }
    }
}

auto Engine::InitState(Hero &hero, std::vector<Entity *> &entities) -> void
{
    entities = std::vector<Entity *>();
    _machine.Clear();

    auto opt_pattern = _map.InitDraw(-1);
    auto pattern = opt_pattern.value();

    createEntities(pattern.positions, entities);

    for (auto brick : pattern.bricks)
    {
        _machine.AddObject(brick);
    }
    for (auto entity : entities)
    {
        _machine.AddObject(entity);
    }

    for (auto position : pattern.positions)
    {
        if (position.entityType == MapEncoding::Hero)
        {
            hero = Hero(position.position.ix, position.position.iy, 1, 1, HERO_MAX_LIVES);
        }
    }
}

auto Engine::Draw(Hero &hero, std::vector<Entity *> &entities) -> void
{
    _map.Draw();

    for (auto entity : entities)
    {
        entity->Draw();
    }

    hero.Draw();
}