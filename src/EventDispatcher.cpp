#include "EventDispatcher.h"


EventDispatcher& EventDispatcher::instance()
{
    static EventDispatcher eventDispatcher;
    return eventDispatcher;
}

EventDispatcher::EventDispatcher()
{

}

EventDispatcher::~EventDispatcher()
{
    for(auto& dispatcher : m_dispatchers)
    {
        delete dispatcher.second;
    }
}


void EventDispatcher::update()
{
    for(auto& dispatcher : m_dispatchers)
    {
        dispatcher.second->update();
    }
}

EventDispatcher::ISpecificDispatcher::~ISpecificDispatcher()
{

}

