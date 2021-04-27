#ifndef HANOI_TOWER_CUBE_H
#define HANOI_TOWER_CUBE_H
#pragma once
#include "HSLAPixel.h"

class Cube{
public:
    Cube(double length, HSLAPixel color);

    double getLength() const;
    void setLength(double length);

    double getVolume() const;
    double getSurfaceArea() const;

private:
    double length_;
    HSLAPixel color_;
};

#endif //HANOI_TOWER_CUBE_H
