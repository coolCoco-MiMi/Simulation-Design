#pragma once

/*
Function: sets up road structure for our cars
Parameters: n - number of cars we want to have
Return value: none
*/
void InitializeRoad(int n);

/*
Function: updates the status of our cars on the road
Parameters: deltaT - time increment we want to use for updates
Return value: none
*/
void UpdateRoad(double deltaT);

/*
Function: provides vehicle position at each time step
Parameters: none
Return value: none
*/
void PrintRoad();
