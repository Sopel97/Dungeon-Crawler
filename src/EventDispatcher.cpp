#include "EventDispatcher.h"

EventDispatcher& EventDispatcher::instance()
{
    static EventDispatcher eventDispatcher;
    return eventDispatcher;
}
