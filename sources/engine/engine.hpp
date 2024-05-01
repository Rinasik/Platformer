#pragma once

#include <vector>
#include <string>
#include <functional>

#include "../map/map.hpp"
#include "../machine/machine.hpp"
#include "../objects/hero/hero.hpp"
#include "../objects/entity/entity.hpp"
#include "../objects/enemies/warrior/warrior.hpp"
#include "../objects/enemies/jumper/jumper.hpp"
#include "../objects/platform/platform.hpp"
#include "../objects/bonus/bonus.hpp"

auto AddHit(Machine &machine, std::unordered_set<Entity *> &entities) -> std::function<void(Hit *)>;

class Engine
{
private:
    Map _map;
    Machine _machine;

    auto createEntities(std::vector<EntityPosition> positions, std::unordered_set<Entity *> &entities) -> void;
    auto removeEntity(Entity *&entity, std::unordered_set<Entity *> &entities) -> void;

public:
    Engine(){};
    Engine(std::string path) : _map(Map(WIDTH, HEIGHT, path)), _machine(Machine()){};

    auto InitState(Hero &hero, std::unordered_set<Entity *> &entities) -> void;
    auto UpdateState(Hero &hero, std::unordered_set<Entity *> &entities) -> void;
    auto Draw(Hero &hero, std::unordered_set<Entity *> &entities) -> void;
};

auto Engine::createEntities(std::vector<EntityPosition> positions, std::unordered_set<Entity *> &entities) -> void
{
    for (auto position : positions)
    {
        if (position.entityType == MapEncoding::Warrior)
        {
            CreateWarrior(position.position, entities);
        }
        else if (position.entityType == MapEncoding::Jumper)
        {
            CreateJumper(position.position, entities);
        }
        else if (position.entityType == MapEncoding::Platform)
        {
            CreatePlatforms(position.position, entities);
        }
    }
}

auto Engine::UpdateState(Hero &hero, std::unordered_set<Entity *> &entities) -> void
{
    for (auto entity : entities)
    {
        if (entity->isDestroyed)
        {
            if (IsEnemy(entity))
            {
                Enemy *enemy = dynamic_cast<Enemy *>(entity);
                std::optional<Bonus *> bonus = enemy->GetBonus();

                if (bonus.has_value())
                {
                    _machine.AddObject(bonus.value());
                    entities.emplace(bonus.value());
                }
            }

            removeEntity(entity, entities);
        }
        else
        {
            entity->Run(_machine.FindNearby(entity->GetShape()));
            _machine.UpdatePosition(entity);
        }
    }
    hero.Run(_machine.FindNearby(hero.GetShape()));

    auto opt_pattern = _map.InitDraw(hero.level);
    if (opt_pattern.has_value())
    {
        auto pattern = opt_pattern.value();
        _machine.Clear();

        entities = std::unordered_set<Entity *>();

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

auto Engine::InitState(Hero &hero, std::unordered_set<Entity *> &entities) -> void
{
    entities = std::unordered_set<Entity *>();
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
            hero = Hero(position.position.ix, position.position.iy, 1, 1, HERO_MAX_LIVES, AddHit(_machine, entities));
        }
    }
}

auto Engine::Draw(Hero &hero, std::unordered_set<Entity *> &entities) -> void
{
    _map.Draw();

    hero.Draw();

    for (auto entity : entities)
    {
        entity->Draw();
    }
}

auto Engine::removeEntity(Entity *&entity, std::unordered_set<Entity *> &entities) -> void
{
    _machine.RemoveObject(entity);
    entities.erase(entity);


    if (entity->type == MapEncoding::Hit)
    {
        Hit *hit = dynamic_cast<Hit *>(entity);

        delete hit;
    }
    else if (entity->type == MapEncoding::Warrior)
    {
        Warrior *warrior = dynamic_cast<Warrior *>(entity);

        delete warrior;
    }
    else if (entity->type == MapEncoding::Jumper)
    {
        Jumper *jumper = dynamic_cast<Jumper *>(entity);

        delete jumper;
    }
}

auto AddHit(Machine &machine, std::unordered_set<Entity *> &entities) -> std::function<void(Hit *)>
{

    auto cb = [&machine, &entities](Hit *hit)
    {
        machine.AddObject(hit);
        entities.emplace(hit);
    };

    return cb;
}