#include <iostream>
#include "Road.h"

using namespace std;

const double deltaT = 0.01;
const double endTime = 40.0;

int main()
{
    InitializeRoad(4);

    for (double t = 0.0; t <= endTime; t += deltaT) {
        UpdateRoad(deltaT);
        cout << t << ", ";
        PrintRoad(); }
}

