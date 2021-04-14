#pragma once

typedef double Time;

class Entity
{
public:
    int GetID();
    Entity();

private:
    int _id;
    static int _nextID;
};