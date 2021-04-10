#include "Wafer.h"
#include <iostream>
using namespace std;

bool wafer[n][n];

bool inCircle(double center_x, double center_y, double radius, double x, double y)
{
    double dist = sqrt(pow(center_x-x,2)+pow(center_y-y,2));
    return dist <= radius;
}
void initializeWafer()
{
    for (int i = 0; i<12; i++){
        for (int j = 0; j<12; j++)
        {
            if (!inCircle(6,6,6,chipSize*i, chipSize*j)) //lower left
                wafer[i][j] = false;
            else if (!inCircle(6, 6, 6, chipSize * (i + 1), chipSize * j))  //upper left
                wafer[i][j] = false;
            else if (!inCircle(6, 6, 6, chipSize * i, chipSize * (j + 1))) //lower right
                wafer[i][j] = false;
            else if (!inCircle(6, 6, 6, chipSize * (i + 1), chipSize * (j + 1))) //uper right
                wafer[i][j] = false;
            else
                wafer[i][j] = true;
        }
    }
}

void generateDefects(int avgNumDefects)
{
    for (int defectNum = 0; defectNum < avgNumDefects; defectNum++)
    {
        double x, y;
        random_device rd;
        mt19937 mt(rd());
        uniform_real_distribution<double> dist(0,1);

        //if defect is not on wafer, regenerate
        do {
            x = dist(mt)*n;  //random*(0,12]
            y = dist(mt)*n;
        } while (!inCircle(6,6,6,x,y));

        int i = (int)(floor(x /chipSize));
        int j = (int)(floor(y/chipSize));
        wafer[i][j] = false;
    }
}

int countGoodChips()
{
    int count = 0;
    for (int i = 0; i<n; i++)
    {
        for (int j = 0; j<n; j++){
            if (wafer[i][j] == true)
            {
                count ++;
            }
        }
    }
    return count;
}

