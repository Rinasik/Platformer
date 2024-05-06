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
#include "../objects/enemies/monster/monster.hpp"
#include "../objects/platform/platform.hpp"
#include "../objects/chest/chest.hpp"
#include "../objects/box/box.hpp"
#include "../objects/bonus/bonus.hpp"

auto AddEntity(Machine &machine, std::unordered_set<Entity *> &entities) -> std::function<void(Entity *)>;

class Engine
{
private:
    Map _map;
    Machine _machine;

    auto createEntities(std::vector<EntityPosition> positions, std::unordered_set<Entity *> &entities) -> void;
    auto removeEntity(Entity *entity, std::unordered_set<Entity *> &entities) -> void;

public:
    Engine(){};
    Engine(std::string path) : _map(Map(WIDTH, HEIGHT, path)), _machine(Machine()){};

    auto InitState(Hero *&hero, std::unordered_set<Entity *> &entities) -> void;
    auto UpdateState(Hero *&hero, std::unordered_set<Entity *> &entities) -> void;
    auto Draw(Hero *&hero, std::unordered_set<Entity *> &entities) -> void;
};

auto Engine::createEntities(std::vector<EntityPosition> positions, std::unordered_set<Entity *> &entities) -> void
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

auto Engine::UpdateState(Hero *&hero, std::unordered_set<Entity *> &entities) -> void
{
    for (auto &entity : entities)
    {
        if (entity->isDestroyed)
        {
            std::optional<Bonus *> bonus = std::nullopt;

            if (IsEnemy(entity))
            {
                Enemy *enemy = dynamic_cast<Enemy *>(entity);
                bonus = enemy->GetBonus();
            }
            else if (entity->type == MapEncoding::Chest)
            {
                Chest *chest = dynamic_cast<Chest *>(entity);
                bonus = chest->GetBonus();
            }
            else if (entity->type == MapEncoding::Box)
            {
                Box *box = dynamic_cast<Box *>(entity);
                bonus = box->GetBonus();

                if (!bonus.has_value())
                {
                    auto shape = box->GetShape();
                    Warrior *warrior = new Warrior(shape.left / DELTA_X, HEIGHT - 1 - shape.bottom / DELTA_Y);

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

    auto opt_pattern = _map.InitDraw(hero->level);
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

        for (auto &position : pattern.positions)
        {
            if (position.entityType == MapEncoding::Hero)
            {
                hero->UpdatePosition(position.position.ix, position.position.iy);
            }
        }
    }
}

auto Engine::InitState(Hero *&hero, std::unordered_set<Entity *> &entities) -> void
{
    for (auto &entity : entities)
    {
        removeEntity(entity, entities);
    }
    entities = std::unordered_set<Entity *>();
    _machine.Clear();

    auto opt_pattern = _map.InitDraw(-1);
    auto pattern = opt_pattern.value();

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
        if (position.entityType == MapEncoding::Hero)
        {
            hero = new Hero(position.position.ix, position.position.iy, 1, 1, HERO_MAX_LIVES, AddEntity(_machine, entities));
            _machine.AddObject(hero);
        }
    }
}

auto Engine::Draw(Hero *&hero, std::unordered_set<Entity *> &entities) -> void
{
    _map.Draw();

    hero->Draw();

    for (auto entity : entities)
    {
        entity->Draw();
    }
}

auto Engine::removeEntity(Entity *entity, std::unordered_set<Entity *> &entities) -> void
{
    _machine.RemoveObject(entity);
    entities.erase(entity);

    if (entity->type == MapEncoding::Archer)
    {
        Archer *enemy = dynamic_cast<Archer *>(entity);
        delete enemy;
    }
    else if (entity->type == MapEncoding::Jumper)
    {
        Jumper *enemy = dynamic_cast<Jumper *>(entity);
        delete enemy;
    }
    else if (entity->type == MapEncoding::Warrior)
    {
        Warrior *enemy = dynamic_cast<Warrior *>(entity);
        delete enemy;
    }
}

auto AddEntity(Machine &machine, std::unordered_set<Entity *> &entities) -> std::function<void(Entity *)>
{
    auto cb = [&machine, &entities](Entity *hit)
    {
        machine.AddObject(hit);
        entities.emplace(hit);
    };

    return cb;
}