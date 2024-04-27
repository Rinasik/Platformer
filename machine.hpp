#pragma once

#include <cmath>
#include <unordered_map>
#include <vector>
#include <set>
#include <cmath>

#include "utils.hpp"
#include "entity.hpp"
#include "constants.hpp"

// Spatial Hashing
class Machine
{
private:
    std::unordered_map<int, std::set<Entity *>> dict;
    std::unordered_map<Entity *, int> objects;

    auto key(int x, int y) -> long long int;

public:
    auto UpdatePosition(Entity *entity) -> void;
    auto AddObject(Entity *entity) -> void;
    auto FindNearby(Shape shape) -> std::vector<Entity *>;
    auto Clear() -> void;
};

auto Machine::key(int x, int y) -> long long int
{
    return ((x + 1) * 73856093) ^ ((y + 1) * 19349663);
}

auto Machine::FindNearby(Shape shape) -> std::vector<Entity *>
{
    auto result = std::vector<Entity *>();

    int left_i = floor(shape.left / DELTA_X) + decimalPart(shape.left / DELTA_X);
    int right_i = ceil(shape.right / DELTA_X) - 1;

    int top_i = ceil(shape.top / DELTA_Y) - 1;
    int bottom_i = floor(shape.bottom / DELTA_Y) + decimalPart(shape.bottom / DELTA_Y);

    int minX = left_i - 1 >= 0 ? left_i - 1 : 0;
    int minY = bottom_i - 1 >= 0 ? bottom_i - 1 : 0;

    int maxX = right_i + 1 >= WIDTH ? WIDTH : right_i + 1;
    int maxY = top_i + 1 >= HEIGHT ? HEIGHT : top_i + 1;

    for (int i = minX; i <= maxX; ++i)
    {
        for (int j = minY; j <= maxY; ++j)
        {
            if (left_i == i && top_i == j)
            {
                continue;
            }

            auto k = key(i, j);
            for (auto vector : dict[k])
            {
                result.push_back(vector);
            }
        }
    }
    return result;
}

auto Machine::AddObject(Entity *entity) -> void
{
    auto shape = entity->GetShape();

    int left_i = floor(shape.left / DELTA_X) + decimalPart(shape.left / DELTA_X);
    int right_i = ceil(shape.right / DELTA_X) - 1;

    int top_i = ceil(shape.top / DELTA_Y) - 1;
    int bottom_i = floor(shape.bottom / DELTA_Y) + decimalPart(shape.bottom / DELTA_Y);

    for (int x = left_i; x <= right_i; ++x)
    {
        for (int y = bottom_i; y <= top_i; ++y)
        {
            int k = key(x, y);
            auto val = dict[k];

            if (dict.find(k) == dict.end())
            {
                dict.emplace(k, std::set<Entity *>());
            }

            dict[k].insert(entity);
            objects.emplace(entity, k);
        }
    }
}

auto Machine::UpdatePosition(Entity *entity) -> void
{
    auto k = objects[entity];

    objects.erase(entity);
    dict[k].erase(entity);

    AddObject(entity);
}

auto Machine::Clear() -> void
{
    dict = std::unordered_map<int, std::set<Entity *>>();
}