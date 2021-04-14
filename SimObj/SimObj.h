#pragma  once

typedef double Time;

class Event
{
public:
    Event(){
    };
    virtual void Execute() = 0;
};

class SimObj
{
public:
    static Time GetCurrentSimTime();
    static void RunSimulation();
    static void RunSimulation(Time endTime);

protected:
    SimObj();
    void ScheduleEventIn(Time deltaTime, Event *ev);
    void ScheduleEventAt(Time time, Event *ev);

private:
    static Time _currentTime;    //maintains the current time for the entire simulation
    class EventSet;              //container for all the scheduled events
    static EventSet _eventSet;  //event set for the entire simulation
};