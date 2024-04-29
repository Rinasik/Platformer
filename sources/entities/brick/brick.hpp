#pragma once

#include "../object/object.hpp"

class Brick : virtual public Object
{
public:
    Brick(int ix, int iy);
};

Brick::Brick(int ix, int iy) : Object{
                                   ix,
                                   iy,
                                   1,
                                   1,
                                   MapEncoding::Brick}
{
}
