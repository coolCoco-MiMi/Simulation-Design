#include <iostream>
#include "Wafer.h"

using namespace  std;

int main()
{
    initializeWafer();
    int count = countGoodChips();

    cout << "Number of chips on wafer = " << count << endl;
    cout <<"(#defects, Average number of good chips)" <<endl;

    for (int numDefects=10; numDefects<=100; numDefects+=10)
    {
        int total = 0;
        for (int i = 1; i<=30; i++)
        {
            initializeWafer();
            generateDefects(numDefects);
            count = countGoodChips();
            total += count;
        }
        cout << numDefects <<", "<<(double) total/30.0 <<endl;
    }
}
