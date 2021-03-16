#include "EulersMethod.h"
#include "Car.h"

Car *CreateCar(double initialPosition, double initialVelocity, double timeGap, double sensitivity)
{
	Car *car = new Car;
	car->positionCS = initialPosition;
	car->velocityCS = initialVelocity;
	car->accelerationCS = 0;
	car->leader = 0;
	car->timeGap = timeGap;
	car->sensitivity = sensitivity;
	return( car);
}

double ControllerAcceleration(Car *car)
{
	return(car->sensitivity*((car->leader->positionCS - car->positionCS) - car->velocityCS * car->timeGap));	// computes acceleration based on distance and time gap
}

void SetLeader(Car *car, Car *leader)
{
	car->leader = leader;
	car->accelerationCS = ControllerAcceleration(car);
}

double MPH2FPS(double mph)
{
	return(mph * 5280 / 3600);
}

double FPS2MPH(double mph)
{
	return(mph * 3600 / 5280);
}

void ComputeNS(Car *car, double deltaT)
{
	static int leaderState = 0;
	static double leaderTimer = 0.0;

	if (car->leader == 0) {		//car is the leader
		switch (leaderState)
		{
			case 0:
				if (car->velocityCS < MPH2FPS(60.0)) {
					car->accelerationNS = 8.82; }			// leader accelerates to 60 mph
				else {
					car->accelerationNS = 0.0;
					leaderState = 1;
					leaderTimer = 0.0; }
				break;
			case 1:
				if (leaderTimer < 10.0) {
					car->accelerationNS = 0.0; }			// leader stays at 60 mph for 10 seconds
				else {
					car->accelerationNS = -8.82;
					leaderState = 2; }
				leaderTimer += deltaT;
				break;
			case 2:
				if (car->velocityCS > 0) {
					car->accelerationNS = -8.82; }			// leader decelerates to 0 mph after 10 seconds
				else {
					car->accelerationNS = 0.0;
					leaderState = 3; }
				break;
			case 3:
				car->accelerationNS = 0.0;					// leader stays at 0 mph
				break;
			default:
				break;
		}
		car->velocityNS = car->velocityCS + car->accelerationCS * deltaT;		// computes next state velocity
		car->positionNS = car->positionCS + car->velocityCS*deltaT; }			// computes next state position
	else {
		double distance = GetPosition(car->leader) - car->positionCS;			// computes distance between leader and current car
		car->accelerationNS = ControllerAcceleration(car);						// computes next state acceleration
		
		car->velocityNS = Euler(car->velocityCS, car->accelerationCS, deltaT);	// computes next state velocity
		car->positionNS = Euler(car->positionCS, car->velocityCS, deltaT); }	// computes next state position
}

void UpdateCS(Car *car)															// updates all current states with computed next states
{
	car->accelerationCS = car->accelerationNS;
	car->velocityCS = car->velocityNS;
	car->positionCS = car->positionNS;
}

double GetPosition(Car *car)
{
	return car->positionCS;
}
