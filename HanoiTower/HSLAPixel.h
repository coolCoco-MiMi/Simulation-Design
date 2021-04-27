#ifndef HANOI_TOWER_HSLAPIXEL_H
#define HANOI_TOWER_HSLAPIXEL_H
#pragma once

#include <iostream>
#include <sstream>

class HSLAPixel{
public:
    double h;
    double s;
    double l;
    double a;

    HSLAPixel();
    HSLAPixel(double hue, double saturation, double luminance);
    HSLAPixel(double hue, double saturation, double luminance, double alpha);

    static HSLAPixel BLUE;
    static HSLAPixel ORANGE;
    static HSLAPixel YELLOW;
    static HSLAPixel PURPLE;

};

#endif //HANOI_TOWER_HSLAPIXEL_H
