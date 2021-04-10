// PID.cpp : Defines the entry point for the console application.
#include <iostream>
#include "EulersMethod.h"
#include "ControllerState.h"


using namespace std;

const double deltaT = 0.01;
const double endTime = 10.0;

int main()
{
    ControllerState *state = initController();
    cout << "0, ";
    PrintState(state);
    for (double t = 0.01; t <= endTime; t += deltaT) {
        ComputeNS(state, deltaT);
        UpdateCS(state);
        cout << t << ", ";
        PrintState(state);
    }
}

