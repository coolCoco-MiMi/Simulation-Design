#include "wafer.h"
using namespace std;

// A two demensiional array to model the wafer environment
bool wafer[n][n];

// Check to see if chip corner is within specified radius
bool inCircle(double center_x, double center_y, double radius, double x, double y)
{
	double dist = sqrt(pow(center_x - x, 2) + pow(center_y - y, 2));
	return dist <= radius;
}

//sets up the wafer per the parameters of the assignment for chip size
void initializeWafer()
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (!inCircle(6, 6, 6, chipSize * i, chipSize * j))wafer[i][j] = false;                   //lower left
			else if (!inCircle(6, 6, 6, chipSize * (i + 1), chipSize * j))wafer[i][j] = false;        //upper left
			else if (!inCircle(6, 6, 6, chipSize * i, chipSize * (j + 1)))wafer[i][j] = false;        //lower right
			else if (!inCircle(6, 6, 6, chipSize * (i + 1), chipSize * (j + 1)))wafer[i][j] = false;  //upper right
			else wafer[i][i] = true;
		}
	}
}

//generates uniformly distributed defects on the wafer
void generateDefects(int avgNumDefects)
{
	for (int defectNum = 0; defectNum < avgNumDefects; defectNum++) 
	{
		double x, y;

		random_device rd;
		mt19937 mt(rd());
		uniform_real_distribution<double> dist(0, 1);

		//if defect is not on wafer, regenerate
		do
		{
			x = dist(mt) * n;  //random (0,12]
			y = dist(mt) * n;  //random (0,12]
		} while (!inCircle(6, 6, 6, x, y));

		int i = (int)(floor(x / chipSize));
		int j = (int)(floor(y / chipSize));
		wafer[i][j] = false;
	}
}

//count the number of chips on the wafer that have no effects
int countGoodChips()
{
	int count = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < 12; j++) {
			if (wafer[i][j] == true) 
			{
				count++;
			}
		}
	}
	return count;
}
