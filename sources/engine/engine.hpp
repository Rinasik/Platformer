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
#include "../objects/enemies/archer/archer.hpp"
#include "../objects/enemies/boss/boss.hpp"
#include "../objects/enemies/monster/monster.hpp"
#include "../objects/platform/platform.hpp"
#include "../objects/chest/chest.hpp"
#include "../objects/box/box.hpp"
#include "../objects/bonus/bonus.hpp"

auto AddEntity(Machine &machine, std::unordered_set<std::shared_ptr<Entity>> &entities) -> std::function<void(std::shared_ptr<Entity>)>;

class Engine
{
private:
    Map _map;
    Machine _machine;

    auto createEntities(std::vector<EntityPosition> positions, std::unordered_set<std::shared_ptr<Entity>> &entities) -> void;
    auto removeEntity(const std::shared_ptr<Entity> &entity, std::unordered_set<std::shared_ptr<Entity>> &entities) -> void;
    auto fillState(std::shared_ptr<Hero> &hero, std::shared_ptr<Boss> &boss, std::unordered_set<std::shared_ptr<Entity>> &entities, MapPattern &pattern) -> void;

public:
    Engine(){};
    Engine(std::string path) : _map(Map(WIDTH, HEIGHT, path)), _machine(Machine()){};

    auto InitState(std::shared_ptr<Hero> &hero, std::shared_ptr<Boss> &boss, std::unordered_set<std::shared_ptr<Entity>> &entities) -> void;
    auto UpdateState(std::shared_ptr<Hero> &hero, std::shared_ptr<Boss> &boss, std::unordered_set<std::shared_ptr<Entity>> &entities) -> void;
    auto Draw(std::shared_ptr<Hero> &hero, std::shared_ptr<Boss> &boss, std::unordered_set<std::shared_ptr<Entity>> &entities) -> void;
};

auto Engine::createEntities(std::vector<EntityPosition> positions, std::unordered_set<std::shared_ptr<Entity>> &entities) -> void
{
    for (auto &position : positions)
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
        else if (position.entityType == MapEncoding::Archer)
        {
            CreateArcher(position.position, entities, AddEntity(_machine, entities));
        }
        else if (position.entityType == MapEncoding::Monster)
        {
            CreateMonster(position.position, entities, AddEntity(_machine, entities));
        }
        else if (position.entityType == MapEncoding::Chest)
        {
            CreateChest(position.position, entities);
        }
        else if (position.entityType == MapEncoding::Box)
        {
            CreateBox(position.position, entities);
        }
        else if (position.entityType == MapEncoding::Bonus)
        {
            CreateKey(position.position, entities);
        }
    }
}

auto Engine::UpdateState(std::shared_ptr<Hero> &hero, std::shared_ptr<Boss> &boss, std::unordered_set<std::shared_ptr<Entity>> &entities) -> void
{
    for (auto &entity : entities)
    {
        if (entity->isDestroyed)
        {
            std::optional<std::shared_ptr<Bonus>> bonus = std::nullopt;

            if (IsEnemy(entity))
            {
                auto enemy = std::dynamic_pointer_cast<Enemy>(entity);
                bonus = enemy->GetBonus();
            }
            else if (entity->type == MapEncoding::Chest)
            {
                auto chest = std::dynamic_pointer_cast<Chest>(entity);
                bonus = chest->GetBonus();
            }
            else if (entity->type == MapEncoding::Box)
            {
                auto box = std::dynamic_pointer_cast<Box>(entity);
                bonus = box->GetBonus();

                if (!bonus.has_value())
                {
                    auto shape = box->GetShape();
                    auto warrior = std::shared_ptr<Warrior>(new Warrior(shape.left / DELTA_X, HEIGHT - 1 - shape.bottom / DELTA_Y));

                    _machine.AddObject(warrior);
                    entities.emplace(warrior);
                }
            }

            if (bonus.has_value())
            {
                _machine.AddObject(bonus.value());
                entities.emplace(bonus.value());
            }

            removeEntity(entity, entities);
        }
        else
        {
            entity->Run(_machine.FindNearby(entity));
            _machine.UpdatePosition(entity);
        }
    }
    hero->Run(_machine.FindNearby(hero));
    _machine.UpdatePosition(hero);

    if (boss)
    {
        if (boss->isDestroyed)
        {
            removeEntity(boss, entities);

            boss = nullptr;
        }
        else
        {
            std::unordered_set<std::shared_ptr<Object>> additionalObjects = std::unordered_set<std::shared_ptr<Object>>();
            additionalObjects.emplace(hero);

            boss->Run(_machine.FindNearby(boss, additionalObjects));
            _machine.UpdatePosition(boss);
        }
    }

    auto opt_pattern = _map.InitDraw(hero->level);

    if (opt_pattern.has_value())
    {
        auto pattern = opt_pattern.value();

        fillState(hero, boss, entities, pattern);
    }
}

auto Engine::InitState(std::shared_ptr<Hero> &hero, std::shared_ptr<Boss> &boss, std::unordered_set<std::shared_ptr<Entity>> &entities) -> void
{
    hero = nullptr;

    auto opt_pattern = _map.InitDraw(-1);
    auto pattern = opt_pattern.value();

    fillState(hero, boss, entities, pattern);
}

auto Engine::fillState(std::shared_ptr<Hero> &hero, std::shared_ptr<Boss> &boss, std::unordered_set<std::shared_ptr<Entity>> &entities, MapPattern &pattern) -> void
{
    for (auto &entity : entities)
    {
        removeEntity(entity, entities);
    }
    boss = nullptr;
    entities = std::unordered_set<std::shared_ptr<Entity>>();
    _machine.Clear();

    createEntities(pattern.positions, entities);

    for (auto &brick : pattern.bricks)
    {
        _machine.AddObject(brick);
    }
    for (auto &entity : entities)
    {
        _machine.AddObject(entity);
    }

    for (auto &position : pattern.positions)
    {
        if (!hero && position.entityType == MapEncoding::Hero)
        {
            hero = std::shared_ptr<Hero>(new Hero(position.position.ix, position.position.iy, 1, 1, HERO_MAX_LIVES, AddEntity(_machine, entities)));
            _machine.AddObject(hero);
        }
        else if (hero && position.entityType == MapEncoding::Hero)
        {
            hero->UpdatePosition(position.position.ix, position.position.iy);
        }
        else if (position.entityType == MapEncoding::Boss)
        {
            boss = std::shared_ptr<Boss>(new Boss(position.position.ix, position.position.iy));
            _machine.AddObject(hero);
        }
    }
}

auto Engine::Draw(std::shared_ptr<Hero> &hero, std::shared_ptr<Boss> &boss, std::unordered_set<std::shared_ptr<Entity>> &entities) -> void
{
    _map.Draw();

    hero->Draw();
    if (boss)
    {
        boss->Draw();
    }

    for (auto &entity : entities)
    {
        entity->Draw();
    }
}

auto Engine::removeEntity(const std::shared_ptr<Entity> &entity, std::unordered_set<std::shared_ptr<Entity>> &entities) -> void
{
    _machine.RemoveObject(entity);
    // entities.erase(entity);
}

auto AddEntity(Machine &machine, std::unordered_set<std::shared_ptr<Entity>> &entities) -> std::function<void(std::shared_ptr<Entity>)>
{
    auto cb = [&machine, &entities](std::shared_ptr<Entity> hit)
    {
        machine.AddObject(hit);
        entities.emplace(hit);
    };

    return cb;
}