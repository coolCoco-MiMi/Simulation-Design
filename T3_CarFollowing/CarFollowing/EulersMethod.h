#pragma once

/*
Function: computes the integral of a function per the Euler numerical integration method
Parameters:
	x - current value of function
	dxdt - derivative of function
	deltaT - time step for integration
Return value: Euler integration value
*/
double Euler(double x, double dxdt, double deltaT);
