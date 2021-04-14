#include "SimObj.h"
#include <iostream>

using namespace std;

class SimObj::EventSet
{
public:
    EventSet()
    {
        _first = 0;  //initialized the pointer to the head EventNode
    }

    void AddEvent(Time time, Event*ev)
    {
        EventNode *en = new EventNode(time, ev);
        if(_first == 0)
        {
            _first = en;
        }
        else if(time < _first->_time)
        {
            en->_next = _first;
            _first = en;
        }
        else
        {
            EventNode *current  = _first;
            while((current->_next == 0) ? false : (current->_next->_time <= time)) //adds node to middle of EventSet
            {
                current = current->_next;
            }
            en->_next = current->_next;
            current->_next = en;
        }
    }
    Event *GetEvent()
    {
        Event *ev = _first->_ev;
        EventNode*en = _first;
        _first = _first->_next;
        return ev;
    }
    void ListEvent()
    {
        Time temp;
        EventNode *node = _first;
        cout <<"Node time " <<node->_next->_time<<endl;
        while (node->_next != 0)
        {
            cout << "Node time " << node->_next->_time << endl;
            node = node->_next;
        }
    }

    Time GetTime()
    {
        return _first->_time;
    }

    bool HasEvent()
    {
        return (_first != 0);
    }

private:
    struct EventNode
    {
        EventNode(Time time, Event*ev)
        {
            _time = time;     //time that this event should be executed
            _ev = ev;         //event associated with this node
            _next = 0;        //pointer to the next event in the sequence of events
        }
        Time _time;
        Event *_ev;
        EventNode *_next;
    };
    EventNode *_first;       //pointer to the first event in the EventSet
};

Time SimObj::GetCurrentSimTime()
{
    return SimObj::_currentTime;
}

void SimObj::RunSimulation()
{
    SimObj::_eventSet.ListEvent();
    while (SimObj::_eventSet.HasEvent())
    {
        SimObj::_currentTime == SimObj::_eventSet.GetTime();
        Event*ev = SimObj::_eventSet.GetEvent();

        //execute the code associated with the event
        //ev->Execute();
    }
}

void SimObj::RunSimulation(Time endTime)
{
    while ((SimObj::_eventSet.HasEvent()) ? (SimObj::_eventSet.GetTime() <=
                                             endTime) : false)
    {
        SimObj::_currentTime = SimObj::_eventSet.GetTime();
        Event *ev = SimObj::_eventSet.GetEvent();

        //execute the code associated with the event
        //ev->Execute();
    }
}

Time SimObj::_currentTime = 0.0;     //initialize simulation time to zero at the start of the simulation
SimObj::EventSet SimObj::_eventSet;  //instantiates the eventSet at the start of the simulation

SimObj::SimObj()
{
}

void SimObj::ScheduleEventIn(Time deltaTime, Event *ev)
{
    _eventSet.AddEvent(_currentTime + deltaTime, ev);
}

void SimObj::ScheduleEventAt(Time time, Event *ev)
{
    _eventSet.AddEvent(time, ev);
}

