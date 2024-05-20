#pragma once

#include <string>
#include <filesystem>
#include <vector>
#include <GL/glut.h>
#include <fstream>
#include <optional>
#include <cstdlib>
#include <map>
#include <regex>

#include "../objects/object/object.hpp"
#include "../objects/brick/brick.hpp"
#include "../objects/exit/exit.hpp"
#include "../objects/magma/magma.hpp"
#include "../utils/constants.hpp"
#include "../utils/utils.hpp"
#include "../texture/texture.hpp"

namespace fs = std::filesystem;

struct MapPattern
{
    std::vector<EntityPosition> positions;
    std::vector<std::shared_ptr<Object>> bricks;
};

class Map
{
private:
    int _currentMapIndex = -1;
    int _height;
    int _width;
    bool _isBossFight;

    std::vector<MapEncoding> _currentMap;

    std::vector<std::vector<int>> _maps = std::vector<std::vector<int>>();

    auto parseMaps(const std::string &path) -> void;

    Texture *_wall;
    Texture *_magma;
    Texture *_background;

public:
    Map(){};
    Map(int width, int height, const std::string &path);

    auto InitDraw(int newMap) -> std::optional<MapPattern>;
    auto Draw() -> void;
};

Map::Map(int width, int height, const std::string &path)
{
    _height = height;
    _width = width;

    _wall = WALL;
    _magma = MAGMA;
    _background = BACKGROUND;

    parseMaps(path);
}

auto Map::InitDraw(int newMap) -> std::optional<MapPattern>
{
    if (newMap != -1 && newMap == _currentMapIndex)
    {
        return std::nullopt;
    }
    if (newMap == -1)
    {
        newMap = 0;
    }

    _currentMapIndex = newMap;
    _isBossFight = false;

    auto map = _maps[newMap];
    std::vector<MapEncoding> mapPattern;

    std::vector<std::shared_ptr<Object>> bricks;
    std::vector<EntityPosition> positions;

    for (int i = 0; i < map.size(); ++i)
    {
        auto point = map[i];

        if (point == -1)
        {
            mapPattern.push_back(MapEncoding::Magma);
            auto magma = std::shared_ptr<Magma>(new Magma(i % _width, i / _width));

            bricks.push_back(magma);
        }
        else if (point == 1)
        {
            mapPattern.push_back(MapEncoding::Brick);
            auto brick = std::shared_ptr<Brick>(new Brick(i % _width, i / _width));

            bricks.push_back(brick);
        }
        else if (point >= 100)
        {
            mapPattern.push_back(MapEncoding::Exit);
            auto exit = std::shared_ptr<Exit>(new Exit(i % _width, i / _width, point - 101));

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
            positions.push_back(EntityPosition{MapEncoding::Warrior, Position{i % _width, i / _width}});
        }
        else if (point == 4)
        {
            positions.push_back(EntityPosition{MapEncoding::Jumper, Position{i % _width, i / _width}});
        }
        else if (point == 5)
        {
            positions.push_back(EntityPosition{MapEncoding::Archer, Position{i % _width, i / _width}});
        }
        else if (point == 6)
        {
            positions.push_back(EntityPosition{MapEncoding::Monster, Position{i % _width, i / _width}});
        }
        else if (point == 10)
        {
            positions.push_back(EntityPosition{MapEncoding::Boss, Position{i % _width, i / _width}});
            _isBossFight = true;
        }
        else if (point == 7)
        {
            positions.push_back(EntityPosition{MapEncoding::Platform, Position{i % _width, i / _width}});
            mapPattern.push_back(MapEncoding::Empty);

            i++;
        }
        else if (point == 8)
        {
            positions.push_back(EntityPosition{MapEncoding::Chest, Position{i % _width, i / _width}});
        }
        else if (point == 9)
        {
            positions.push_back(EntityPosition{MapEncoding::Box, Position{i % _width, i / _width}});
        }
        else if (point == -10)
        {
            positions.push_back(EntityPosition{MapEncoding::Bonus, Position{i % _width, i / _width}});
        }
        else if (point == -2)
        {
            positions.push_back(EntityPosition{MapEncoding::BreakingBrick, Position{i % _width, i / _width}});
        }
        else if (point == -3)
        {
            positions.push_back(EntityPosition{MapEncoding::TemporaryPlatform, Position{i % _width, i / _width}});
        }
    }

    _currentMap = mapPattern;

    return MapPattern{positions, bricks};
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
                unsigned int texture = _background->GetTexture();
                if (_isBossFight)
                {
                    if (std::rand() % 10 >= 8)
                    {
                        texture = _wall->GetTexture();
                    }
                }

                glBindTexture(GL_TEXTURE_2D, texture);
                glColor4f(0.3f, 0.3f, 0.3f, 1.0f);
            }
            else if (block == MapEncoding::Brick)
            {
                glBindTexture(GL_TEXTURE_2D, _wall->GetTexture());
                glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
            }
            else if (block == MapEncoding::Exit)
            {
                glColor4f(1.0f, 1.0f, 0, 1.0f);
                glBegin(GL_QUADS);

                glVertex2f((j * DELTA_X) - 1.f, -DELTA_Y * i + 1.f);
                glVertex2f((j * DELTA_X) - 1.f, -DELTA_Y * (i + 1) + 1.f);
                glVertex2f((j + 1) * DELTA_X - 1.f, -DELTA_Y * (i + 1) + 1.f);
                glVertex2f((j + 1) * DELTA_X - 1.f, -DELTA_Y * i + 1.f);

                glEnd();
                continue;
            }
            else if (block == MapEncoding::Magma)
            {
                glBindTexture(GL_TEXTURE_2D, _magma->GetTexture());
                glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
            }

            glEnable(GL_TEXTURE_2D);

            glBegin(GL_QUADS);

            glTexCoord2f(0, 1);
            glVertex2f((j * DELTA_X) - 1.f, -DELTA_Y * i + 1.f);

            glTexCoord2f(0, 0);
            glVertex2f((j * DELTA_X) - 1.f, -DELTA_Y * (i + 1) + 1.f);

            glTexCoord2f(1, 0);
            glVertex2f((j + 1) * DELTA_X - 1.f, -DELTA_Y * (i + 1) + 1.f);

            glTexCoord2f(1, 1);
            glVertex2f((j + 1) * DELTA_X - 1.f, -DELTA_Y * i + 1.f);

            glEnd();

            glDisable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
}

auto Map::parseMaps(const std::string &path) -> void
{
    auto temp = std::map<int, std::vector<int>>();
    std::regex number_regex("\\d+");
    std::smatch number;

    for (const auto &entry : fs::directory_iterator(path))
    {
        auto stream = std::ifstream(entry.path());

        auto empty = std::vector<int>();

        for (long long int i = 0; i < _height; ++i)
        {
            for (long long int j = 0; j < _width; ++j)
            {
                long long int tmp;
                stream >> tmp;

                empty.push_back(ParseHash(i, j, tmp));
            }
        }
        auto path_to_file = entry.path().string();
        if (std::regex_search(path_to_file, number, number_regex))
        {
            temp.emplace(std::stoi(number[0]), empty);
        }
    }

    for (auto &temp_map : temp)
    {
        _maps.push_back(temp_map.second);
    }
}