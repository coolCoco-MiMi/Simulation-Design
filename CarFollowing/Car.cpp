#include <iostream>
#include "EulersMethod.h"
#include "Car.h"

using namespace std;

Car::Car(double initialPosition, double initialVelocity, double timeGap, double sensitivity)
{
    positionCS = initialPosition;
    velocityCS = initialVelocity;
    accelerationCS = 0;
    _leader = 0;
    this->timeGap = timeGap;
    this->sensitivity = sensitivity;
}

double Car::ControllerAcceleration()
{
    return(sensitivity*((_leader->positionCS - positionCS) - velocityCS*timeGap));
}

void Car::SetLeader(Car *leader)
{
    _leader = leader;
    accelerationCS = ControllerAcceleration();
}

double Car::MPH2FPS(double mph)
{
    return(mph * 5280 / 3600);
}

double Car::FPS2MPH(double mph)
{
    return(mph * 3600 / 5280);
}

void Car::ComputeNS(double deltaT)
{
    static int leaderState = 0;
    static double leaderTimer = 0.0;

    if (_leader == 0) {		//car is the leader
        switch (leaderState)
        {
            case 0:
                if (velocityCS < MPH2FPS(60.0)) {
                    accelerationNS = 8.82; }
                else {
                    accelerationNS = 0.0;
                    leaderState = 1;
                    leaderTimer = 0.0; }
                break;
            case 1:
                if (leaderTimer < 10.0) {
                    accelerationNS = 0.0; }
                else {
                    accelerationNS = -8.82;
                    leaderState = 2; }
                leaderTimer += deltaT;
                break;
            case 2:
                if (velocityCS > 0) {
                    accelerationNS = -8.82; }
                else {
                    accelerationNS = 0.0;
                    leaderState = 3; }
                break;
            case 3:
                accelerationNS = 0.0;
                break;
            default:
                break;
        }
        velocityNS = velocityCS + accelerationCS * deltaT;
        positionNS = positionCS + velocityCS*deltaT; }
    else {
        double distance = _leader->GetPosition() - positionCS;
        accelerationNS = ControllerAcceleration();
        velocityNS = Euler(velocityCS, accelerationCS, deltaT);
        positionNS = Euler(positionCS, velocityCS, deltaT); }
}

void Car::UpdateCS()
{
    accelerationCS = accelerationNS;
    velocityCS = velocityNS;
    positionCS = positionNS;
}

double Car::GetPosition()
{
    return positionCS;
}

