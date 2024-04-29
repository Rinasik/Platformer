#pragma once

#include "../object/object.hpp"

class Exit : virtual public Object
{
public:
    int mapNumber;

    Exit(int ix, int iy, int mapNumber);
};

Exit::Exit(int ix, int iy, int mapNumber) : Object{
                                                ix,
                                                iy,
                                                1,
                                                1,
                                                MapEncoding::Exit}

{
    this->mapNumber = mapNumber;
}