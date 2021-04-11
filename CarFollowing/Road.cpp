#include <iostream>
#include "Car.h"

using namespace std;

typedef Car *Carptr;
Carptr *cars;
int numCars;

void InitializeRoad(int n)
{
    numCars = n;
    cars = new Carptr[n];  //n pointers
    for (int i = 0; i<n; i++)
    {
        cars[i] = new Car(-(double)i*4.0, 0.0, 2.0, 0.7);}
    for(int i=1; i<n; i++)
    {
        cars[i]->SetLeader(cars[i-1]);
    }
}

void UpdateRoad(double deltaT)
{
    for (int i=0; i<numCars; i++){
        cars[i]->ComputeNS(deltaT);
    }
    for (int i = 0; i < numCars; i++) {
        cars[i]->UpdateCS();}
}

void PrintRoad()
{
    for (int i = 0; i < numCars; i++) {
        cout << cars[i]->GetPosition() << ", ";
    }
    cout << endl;
}