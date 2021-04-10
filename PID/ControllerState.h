#pragma once

struct ControllerState
{
    double positionCS;
    double positionNS;
    double velocityCS;
    double velocityNS;
    double accelerationCS;
    double accelerationNS;
    double errorCS;
    double errorNS;
    double integralErrorCS;
    double integralErrorNS;
    double derivErrorCS;
    double derivErrorNS;
    double currentCS;
    double currentNS;
};

//return value: pointer to controller state
ControllerState *initController();

//compute the next state of the controller
void ComputeNS(ControllerState *state, double deltaT);

//update
void UpdateCS(ControllerState *state);

//print
void PrintState(ControllerState *state);

