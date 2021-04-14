#include "Queue.h"

Queue::Queue()
{
}

class Queue::ArriveEvent:public Event
{
public: ArriveEvent(Queue*queue, Entity *en)
    {
        _queue = queue;
        _en = en;
    }
    void Execute()
    {
    _queue->Arrive(_en);
    }

private:
    Queue *_queue;
    Entity *_en;
};

void Queue::Arrive(Entity * en)
{
}

void Queue::ScheduleArrivalIn(Time deltaTime, Entity *en)
{
    ScheduleEventIn(deltaTime, new ArriveEvent(this, en));
}
