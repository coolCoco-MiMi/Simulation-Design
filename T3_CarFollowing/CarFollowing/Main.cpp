#include <iostream>
#include "Road.h"

using namespace std;

const double deltaT = 0.01;
const double endTime = 40.0;

int main()
{
	InitializeRoad(4); // creates cars and positions them on the road

	for (double t = 0.0; t <= endTime; t += deltaT) {
		UpdateRoad(deltaT);
		cout << t << ", ";
		PrintRoad(); }

	system("pause");
	return 1;
}
