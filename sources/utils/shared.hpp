#pragma once

#include "constants.hpp"

struct Shape
{
    double top;
    double bottom;
    double left;
    double right;
};

struct Position
{
    int ix;
    int iy;
};

struct EntityPosition
{
    MapEncoding entityType;
    Position position;
};