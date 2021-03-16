
#pragma once

struct Car
{
	double positionCS; // current position
	double positionNS; // next position
	double velocityCS; // current velocity
	double velocityNS; // next velocity
	double accelerationCS; // current acceleration
	double accelerationNS; // next acceleration
	Car *leader; // pointer to leader
	double timeGap;
	double sensitivity;
};

/*
Function: creates a car data structure for each car in the simulation
Parameters:
	initialPosition - initial position of this car
	intialVelocity - intial velocity of this car
	timeGap - time we want to stay behind car in front of this vehicle
	sensitivity - how quickly this car responds to changes in time to vehicle in front
Return value: pointer to car structure
*/
Car *CreateCar(double initialPosition, double initialVelocity, double timeGap, double sensitivity);

/*
Function: sets the leader vehicle on the road and its driving patern
Parameters: 
	*car - pointer to car we want to designate as the leader
	*leader - pointer to leader data structure
Return value: none
*/
void SetLeader(Car *car, Car *leader);


/*
Function: computes the next position of each car
Parameters:
	*car - pointer to car to update
	deltaT - time increment for next position update
Return value: none
*/
void ComputeNS(Car *car, double deltaT);

/*
Function: updates each cars state from that calculated by the next state method
Parameters: *car - car to update current state
Return value: none
*/
void UpdateCS(Car *car);

/*
Function: report position of a car
Parameters: *car - car for which to report position
Return value: car's position
*/
double GetPosition(Car *car);
