#pragma once

#include "../object/object.hpp"

class Magma : virtual public Object
{
public:
    Magma(int ix, int iy);
};

Magma::Magma(int ix, int iy) : Object{
                                   (double)ix,
                                   (double)iy,
                                   1,
                                   1,
                                   MapEncoding::Magma}
{
}