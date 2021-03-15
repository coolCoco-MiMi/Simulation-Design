#pragma once
#include <cmath>
#include <random>

#define n 12               //define size diameter
#define chipSize 1.0       //define chip size

/*
Functionality: sets up the wafer per the parameters of the assignment for chip size

Parameters: none

Return value: none
*/

void initializeWafer();

/*
Functionality: generates uniformly distributed defects on thw wafer

Parameters: avgNumDefects - the number of defects you want to generate

Return value: number of defects
*/

void generateDefects(int avgNumDefects);

/*
Functionality: counts the number of chips on the wafer that have no defects

Parameters: none

Return value: number of good chips
*/

int countGoodChips();