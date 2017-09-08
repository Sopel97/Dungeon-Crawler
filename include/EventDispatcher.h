#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include <map>
#include <vector>
#include <list>
#include <functional>
#include <queue>
#include <typeindex>
#include <string>

class EventDispatcher
{
public:
    template <class EventType>
    class EventCallbackHandle
    {
    public:
        EventCallbackHandle(std::list<std::function<void(const EventType& event)>>& allCallbacks, typename std::list<std::function<void(const EventType& event)>>::iterator callback);
        ~EventCallbackHandle();
        void unsubscribe();

    protected:
        std::list<std::function<void(const EventType& event)>>& m_allCallbacks;
        typename std::list<std::function<void(const EventType& event)>>::iterator m_callback; //iterator to where the callback is strored
    };

    static EventDispatcher& instance();

    template <class EventType>
    EventCallbackHandle<EventType> subscribe(std::function<void(const EventType&)>&& callback)
    {
        return SpecificDispatcher<EventType>::subscribe(std::move(callback));
    }

    template <class EventType, class... Arguments>
    void broadcast(Arguments&& ... args)
    {
        SpecificDispatcher<EventType>::broadcast(EventType(std::forward<Arguments>(args)...));
    }

    template <class EventType, class... Arguments>
    void enqueue(Arguments&& ... args)
    {
        SpecificDispatcher<EventType>::enqueue(EventType(std::forward<Arguments>(args)...));
    }

    template <class EventType>
    void update()
    {
        SpecificDispatcher<EventType>::update<EventType>();
    }

protected:

    EventDispatcher() = default;

    template<typename EventType>
    class SpecificDispatcher
    {
    public:

        static EventCallbackHandle<EventType> subscribe(std::function<void(const EventType&)>&& callback);

        static void broadcast(EventType&& event);

        static void enqueue(EventType&& event);

        static void update();

    protected:

        static std::list<std::function<void(const EventType& event)>>& callbacks();
        static std::queue<EventType>& eventQueue();
    };

};

class Event
{
    //maybe something in the future
};


template <class EventType>
EventDispatcher::EventCallbackHandle<EventType>::EventCallbackHandle(std::list<std::function<void(const EventType& event)>>& allCallbacks, typename std::list<std::function<void(const EventType& event)>>::iterator callback) :
    m_allCallbacks(allCallbacks),
    m_callback(callback)
{
}

template <class EventType>
EventDispatcher::EventCallbackHandle<EventType>::~EventCallbackHandle()
{
    unsubscribe();
}
template <class EventType>
void EventDispatcher::EventCallbackHandle<EventType>::unsubscribe()
{
    if (m_callback == m_allCallbacks.end() || m_allCallbacks.empty()) return;
    m_allCallbacks.erase(m_callback);
    m_callback = m_allCallbacks.end();
}



template <typename EventType>
EventDispatcher::EventCallbackHandle<EventType> EventDispatcher::SpecificDispatcher<EventType>::subscribe(std::function<void(const EventType&)>&& callback)
{
    callbacks().push_back(std::move(callback));
    return EventCallbackHandle<EventType>(callbacks(), std::prev(callbacks().end(), 1));
}

template <typename EventType>
void EventDispatcher::SpecificDispatcher<EventType>::broadcast(EventType&& event)
{
    for(const auto& callback : callbacks())
    {
        callback(std::forward<EventType>(event));
    }
}

template <typename EventType>
void EventDispatcher::SpecificDispatcher<EventType>::enqueue(EventType&& event)
{
    eventQueue().emplace(std::forward<EventType>(event));
}

template <typename EventType>
void EventDispatcher::SpecificDispatcher<EventType>::update()
{
    while(!eventQueue().empty())
    {
        const EventType& event = eventQueue().front();
        for(const auto& callback : m_callbacks)
        {
            callback(event);
        }
        eventQueue().pop();
    }
}


template <typename EventType>
std::list<std::function<void(const EventType& event)>>& EventDispatcher::SpecificDispatcher<EventType>::callbacks()
{
    static std::list<std::function<void(const EventType& event)>> c;
    return c;
}

template <typename EventType>
std::queue<EventType>& EventDispatcher::SpecificDispatcher<EventType>::eventQueue()
{
    static std::queue<EventType> q;
    return q;
}


#endif // EVENTDISPATCHER_H
