#pragma once

class Car
{
private:
    //computes the needed acceleration
    double ControllerAcceleration();
    //converts miles per hour to feet per second
    double MPH2FPS(double  mph);
    //converts feet per second to miles per hour
    double FPS2MPH(double mph);

    double positionCS;
    double positionNS;
    double velocityCS;
    double velocityNS;
    double accelerationCS;
    double accelerationNS;
    Car *_leader;
    double timeGap;
    double sensitivity;

public:
    Car(double initialPosition, double initialVelocity, double timeGap, double sensitivity);
    void SetLeader(Car *leader);
    void ComputeNS(double deltaT);
    void UpdateCS();
    double GetPosition();
};
