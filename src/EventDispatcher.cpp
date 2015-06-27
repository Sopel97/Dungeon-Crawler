#include "EventDispatcher.h"


template<typename EventType>
EventDispatcher::EventCallbackHandle<EventType>::EventCallbackHandle(std::list<std::function<void(const EventType& event)>>& allCallbacks, typename std::list<std::function<void(const EventType& event)>>::iterator callback) :
    m_allCallbacks(&allCallbacks),
    m_callback(callback)
{

}
template<typename EventType>
void EventDispatcher::EventCallbackHandle<EventType>::unsubscribe()
{
    m_allCallbacks->erase(m_callback);
}

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

