#include "EulersMethod.h"

double Euler(double x, double dxdt, double deltaT)
{
    return(x + deltaT*dxdt);
}

