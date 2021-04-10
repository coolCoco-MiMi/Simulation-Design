#include "ControllerState.h"
#include "Parameters.h"
#include "EulersMethod.h"
#include <iostream>

using namespace std;

ControllerState *initController()
{
    ControllerState *state = new ControllerState;
    state->currentCS = 1.5;
    state->positionCS = 5.1;
    state->velocityCS = 0;
    double k = m*g*x0*x0/(i0*i0);
    state->accelerationCS = (m*g)-(k*state->currentCS*state->currentCS) / (state->positionCS*state->positionCS) / m;
    state->errorCS = x0 - state->positionCS;
    state->integralErrorCS = 0;
    state->derivErrorCS = 0;
    return (state);
}

void ComputeNS(ControllerState *state, double deltaT)
{
    state->positionNS = state->positionCS+(state->velocityCS*deltaT);
    state->velocityNS = state->velocityCS+(state->accelerationCS*deltaT);
    state->errorNS = x0 - state->positionNS;
    state->integralErrorNS = Euler(state->integralErrorCS, state->errorCS, deltaT);
    state->derivErrorNS = (state->errorNS-state->errorCS) / deltaT;
    state->currentNS = i0 - ((P*state->errorNS) + (I*state->integralErrorNS) + (D*state->derivErrorNS));
    double k = m*g*x0*x0/(i0*i0);
    state->accelerationNS = (m*g)-(k*state->currentNS*state->currentNS) / (state->positionNS*state->positionNS) / m;
}

void UpdateCS(ControllerState *state)
{
    state->accelerationCS = state->accelerationNS;
    state->velocityCS = state->velocityNS;
    state->positionCS = state->positionNS;
    state->derivErrorCS = state->derivErrorNS;
    state->errorCS = state->errorNS;
    state->integralErrorCS = state->integralErrorNS;
    state->currentCS = state->currentNS;
}

void PrintState(ControllerState *state)
{
    cout << state->currentCS             << ", ";
    cout << state->positionCS            << ", ";
    cout << state->velocityCS            << ", ";
    cout << state->accelerationCS        << ", ";
    cout << state->errorCS               << ", ";
    cout << state->integralErrorCS       << ", ";
    cout << state->derivErrorCS          << ", ";
    cout << endl;

}
