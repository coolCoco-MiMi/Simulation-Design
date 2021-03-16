#include <iostream>
#include "Road.h"
#include "Car.h"

using namespace std;

typedef Car *CarPtr;			//assigns a pointer to a structure
CarPtr *cars;					//declares a car pointer
int numCars;

void InitializeRoad(int n)
{
	numCars = n;
	cars = new CarPtr[n]; // creates an array of car structures
	for (int i = 0; i < n; i++) {
		cars[i] = CreateCar(-(double)i*4.0, 0.0, 2.0, 0.7); } //(initial position, initial velocity, time gap, sensitivity)
	for (int i = 1; i < n; i++) {
		SetLeader(cars[i], cars[i - 1]); } // sets the order of the cars
}

void UpdateRoad(double deltaT)
{
	for (int i = 0; i < numCars; i++) {
		ComputeNS(cars[i], deltaT); } // computes each car's next state
	for (int i = 0; i < numCars; i++) {
		UpdateCS(cars[i]); } // updates car's current state to the next state
}

void PrintRoad()
{
	for (int i = 0; i < numCars; i++) {
		cout << GetPosition(cars[i]) << ", "; } // prints out each car's position to the console
	cout << endl;
}
