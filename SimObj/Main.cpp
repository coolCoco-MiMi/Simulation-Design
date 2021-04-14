#include "SimObj.h"
#include "Queue.h"
#include <iostream>

using namespace std;

int main()
{
    Queue queue;
    queue.ScheduleArrivalIn(0.0, new Entity());
    queue.ScheduleArrivalIn(1.0, new Entity());
    queue.ScheduleArrivalIn(2.0, new Entity());
    queue.ScheduleArrivalIn(1.5, new Entity());
    queue.ScheduleArrivalIn(3.0, new Entity());
    SimObj::RunSimulation();
}

