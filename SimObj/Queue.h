#pragma once
#include "Entity.h"
#include "SimObj.h"

class Queue:public SimObj
{
public:
    Queue();

    void ScheduleArrivalIn(Time deltaTime, Entity*en);

private:

    class ArriveEvent;

    void Arrive(Entity*en);
};