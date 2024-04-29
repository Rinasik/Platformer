#pragma once

#include "../../utils/constants.hpp"
#include "../../utils/utils.hpp"
#include "../../utils/shared.hpp"


class Object
{
protected:
    double _x = 0;
    double _y = 0;

    double _sizeX;
    double _sizeY;

public:
    MapEncoding type;

    Object(){};
    Object(int ix, int iy, double sizeX, double sizeY, MapEncoding type);
    virtual ~Object() = default;

    auto GetShape() -> Shape;
};

Object::Object(int ix, int iy, double sizeX, double sizeY, MapEncoding type) : _x(multiply(ix, DELTA_X)), _y(multiply(HEIGHT - iy - 1, DELTA_Y)), _sizeX(sizeX), _sizeY(sizeY), type(type) {}

inline auto Object::GetShape() -> Shape
{
    return Shape{add(multiply(_sizeY, DELTA_Y), _y), _y, _x, add(multiply((_sizeX), DELTA_X), _x)};
}