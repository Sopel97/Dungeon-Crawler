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
    template<typename EventType>
    class EventCallbackHandle
    {
    public:
        EventCallbackHandle(std::list<std::function<void(const EventType& event)>>& allCallbacks, typename std::list<std::function<void(const EventType& event)>>::iterator callback);
        void unsubscribe();
    protected:
        std::list<std::function<void(const EventType& event)>>* m_allCallbacks;
        const typename std::list<std::function<void(const EventType& event)>>::iterator m_callback; //iterator to where the callback is strored
    };

    static EventDispatcher& instance();

    ~EventDispatcher();

    template <class EventType>
    EventCallbackHandle<EventType> subscribe(std::function<void(const EventType&)>&& callback);

    template <class EventType, class... Arguments>
    void broadcast(Arguments&& ... args);

    template <class EventType, class... Arguments>
    void enqueue(Arguments&& ... args);

    void update();

protected:

    EventDispatcher();

    class ISpecificDispatcher
    {
    public:
        virtual ~ISpecificDispatcher();

        virtual void update() = 0;
    };

    template<typename EventType>
    class SpecificDispatcher : public ISpecificDispatcher
    {
    public:
        virtual ~SpecificDispatcher();

        EventCallbackHandle<EventType> subscribe(std::function<void(const EventType&)>&& callback);

        template <typename... Arguments>
        void broadcast(Arguments&& ... args);

        template <typename... Arguments>
        void enqueue(Arguments&& ... args);

        virtual void update();

    protected:
        std::queue<EventType> m_eventQueue;
        std::list<std::function<void(const EventType& event)>> m_callbacks; //list is in order to avoid iterator invalidation which allows us to do unsubscribing in nice way
    };

    template<typename EventType>
    SpecificDispatcher<EventType>* getSpecificDispatcher();

protected:
    std::map<std::type_index, ISpecificDispatcher*> m_dispatchers;
};

class Event
{
    //maybe something in the future
};

//Template members definitions

template <class EventType>
EventDispatcher::EventCallbackHandle<EventType> EventDispatcher::subscribe(std::function<void(const EventType&)>&& callback)
{
    return getSpecificDispatcher<EventType>()->subscribe(std::move(callback));
}

template <class EventType, class... Arguments>
void EventDispatcher::broadcast(Arguments&& ... args)
{
    getSpecificDispatcher<EventType>()->broadcast(EventType(std::forward<Arguments>(args)...));
}

template <class EventType, class... Arguments>
void EventDispatcher::enqueue(Arguments&& ... args)
{
    getSpecificDispatcher<EventType>()->enqueue(EventType(std::forward<Arguments>(args)...));
}


template<typename EventType>
EventDispatcher::EventCallbackHandle<EventType> EventDispatcher::SpecificDispatcher<EventType>::subscribe(std::function<void(const EventType&)>&& callback)
{
    m_callbacks.push_back(std::move(callback));
    return std::next(m_callbacks.end(), -1);
}

template<typename EventType>
template <typename... Arguments>
void EventDispatcher::SpecificDispatcher<EventType>::broadcast(Arguments&& ... args)
{
    EventType event(std::forward<Arguments>(args)...);
    for(const auto& callback : m_callbacks)
    {
        callback(event);
    }
}

template<typename EventType>
template <typename... Arguments>
void EventDispatcher::SpecificDispatcher<EventType>::enqueue(Arguments&& ... args)
{
    m_eventQueue.push(EventType(std::forward<Arguments>(args)...));
}

template<typename EventType>
void EventDispatcher::SpecificDispatcher<EventType>::update()
{
    while(!m_eventQueue.empty())
    {
        const EventType& event = m_eventQueue.front();
        for(const auto& callback : m_callbacks)
        {
            callback(event);
        }
        m_eventQueue.pop();
    }
}


template<typename EventType>
EventDispatcher::SpecificDispatcher<EventType>* EventDispatcher::getSpecificDispatcher()
{
    std::type_index eventTypeIndex = std::type_index(typeid(EventType));
    auto iter = m_dispatchers.find(eventTypeIndex);
    if(iter == m_dispatchers.end())
    {
        SpecificDispatcher<EventType>* specificDispatcher = new SpecificDispatcher<EventType>();
        m_dispatchers.insert(std::make_pair(eventTypeIndex, specificDispatcher));
        return specificDispatcher;
    }
    else
    {
        return static_cast<SpecificDispatcher<EventType>*>(iter->second);
    }
}

#endif // EVENTDISPATCHER_H
