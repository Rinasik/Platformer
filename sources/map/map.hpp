#pragma once

#include <string>
#include <filesystem>
#include <vector>
#include <GL/glut.h>
#include <fstream>
#include <optional>

#include "../entities/object/object.hpp"
#include "../entities/brick/brick.hpp"
#include "../entities/exit/exit.hpp"
#include "../entities/magma/magma.hpp"
#include "../utils/constants.hpp"
#include "../utils/utils.hpp"

namespace fs = std::filesystem;

struct Pattern
{
    std::vector<EntityPosition> positions;
    std::vector<Object *> bricks;
};

class Map
{
private:
    int _currentMapIndex = -1;
    int _height;
    int _width;

    std::vector<MapEncoding> _currentMap;

    std::vector<std::vector<int>> _maps;

    auto parseMaps(const std::string &path) -> void;

public:
    Map(int width, int height, const std::string &path);

    auto InitDraw(int newMap) -> std::optional<Pattern>;
    auto Draw() -> void;
};

Map::Map(int width, int height, const std::string &path)
{
    _height = height;
    _width = width;

    parseMaps(path);
}

auto Map::InitDraw(int newMap) -> std::optional<Pattern>
{
    if (newMap == _currentMapIndex)
    {
        return std::nullopt;
    }
    _currentMapIndex = newMap;

    auto map = _maps[newMap];
    std::vector<MapEncoding> mapPattern;

    std::vector<Object *> bricks;
    std::vector<EntityPosition> positions;

    for (int i = 0; i < map.size(); ++i)
    {
        auto point = map[i];

        if (point == -1)
        {
            mapPattern.push_back(MapEncoding::Magma);
            Object *magma = new Magma(i % _width, i / _width);

            bricks.push_back(magma);
        }
        else if (point == 1)
        {
            mapPattern.push_back(MapEncoding::Brick);
            Object *brick = new Brick(i % _width, i / _width);

            bricks.push_back(brick);
        }
        else if (point >= 100)
        {
            mapPattern.push_back(MapEncoding::Exit);
            Object *exit = new Exit(i % _width, i / _width, point - 101);

            bricks.push_back(exit);
        }
        else
        {
            mapPattern.push_back(MapEncoding::Empty);
        }

        // Заполнение живых сущностей
        if (point == 2)
        {
            positions.push_back(EntityPosition{MapEncoding::Hero, Position{i % _width, i / _width}});
        }
        else if (point == 3)
        {
            positions.push_back(EntityPosition{MapEncoding::Enemy, Position{i % _width, i / _width}});
        }
        else if (point == 4)
        {
            positions.push_back(EntityPosition{MapEncoding::Platform, Position{i % _width, i / _width}});
            mapPattern.push_back(MapEncoding::Empty);

            i++;
        }
    }

    _currentMap = mapPattern;

    return Pattern{positions, bricks};
}

auto Map::Draw() -> void
{
    for (int i = 0; i < _height; ++i)
    {
        for (int j = 0; j < _width; ++j)
        {
            auto block = _currentMap[j + i * _width];

            if (block == MapEncoding::Empty)
            {
                continue;
            }

            if (block == MapEncoding::Brick)
            {
                glColor3f(0.1f, 0.1f, 0.1f);
            }
            else if (block == MapEncoding::Exit)
            {
                glColor3f(1.0f, 1.0f, 0);
            }
            else if (block == MapEncoding::Magma)
            {
                glColor3f(1.0f, 0.65f, 0);
            }
            glBegin(GL_QUADS);

            glVertex2f((j * DELTA_X) - 1.f, -DELTA_Y * i + 1.f);
            glVertex2f((j * DELTA_X) - 1.f, -DELTA_Y * (i + 1) + 1.f);
            glVertex2f((j + 1) * DELTA_X - 1.f, -DELTA_Y * (i + 1) + 1.f);
            glVertex2f((j + 1) * DELTA_X - 1.f, -DELTA_Y * i + 1.f);

            glEnd();
        }
    }
}

auto Map::parseMaps(const std::string &path) -> void
{
    for (const auto &entry : fs::directory_iterator(path))
    {
        auto stream = std::ifstream(entry.path());

        auto empty = std::vector<int>();
        for (int i = 0; i < _height; ++i)
        {
            for (int j = 0; j < _width; ++j)
            {
                int tmp;
                stream >> tmp;

                empty.push_back(tmp);
            }
        }

        _maps.push_back(empty);
    }
}