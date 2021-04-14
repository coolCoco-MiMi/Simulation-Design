#include "Entity.h"

int Entity::_nextID = 1;

int Entity::GetID()
{
    return _id;
}

Entity::Entity()
{
    _id = _nextID;
}
