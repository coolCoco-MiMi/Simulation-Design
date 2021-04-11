#pragma once

/*
Function: compute the integral of a function per the Euler method
Parameters:
	x - current value of the function
	dxdt - function to be evaluated
	deltaT - time step
Return: Euler integration value
*/
double Euler(double x, double dxdt, double deltaT);
