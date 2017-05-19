#include "..\include\SfmlEventHandler.h"

SfmlEventHandler::EventResult SfmlEventHandler::EventResult::setTakeFocus(bool opt) const
{
    return EventResult{ opt, consumeEvent };
}
SfmlEventHandler::EventResult SfmlEventHandler::EventResult::setConsumeEvent(bool opt) const
{
    return EventResult{ takeFocus, opt };
}
SfmlEventHandler::EventContext SfmlEventHandler::EventContext::setIsFocused(bool opt) const
{
    return EventContext{ opt, isMouseOver };
}
SfmlEventHandler::EventContext SfmlEventHandler::EventContext::setIsMouseOver(bool opt) const
{
    return EventContext{ isFocused, opt };
}

SfmlEventHandler::EventResult SfmlEventHandler::dispatch(sf::Event& event, SfmlEventHandler::EventContext context, const ls::Vec2I& mousePos)
{
    switch (event.type)
    {
    case sf::Event::TextEntered:
        return this->onTextEntered(event.text, context);

    case sf::Event::KeyPressed:
        return this->onKeyPressed(event.key, context);

    case sf::Event::KeyReleased:
        return this->onKeyReleased(event.key, context);

    case sf::Event::MouseWheelMoved:
        return this->onMouseWheelMoved(event.mouseWheel, context);

    case sf::Event::MouseButtonPressed:
        return this->onMouseButtonPressed(event.mouseButton, context);

    case sf::Event::MouseButtonReleased:
        return this->onMouseButtonReleased(event.mouseButton, context);

    case sf::Event::MouseMoved:
        return this->onMouseMoved(event.mouseMove, context);

    default:
        return { false, false };
    }
}
SfmlEventHandler::EventResult SfmlEventHandler::onTextEntered(sf::Event::TextEvent& event, EventContext context)
{
	return{ false, false };
}
SfmlEventHandler::EventResult SfmlEventHandler::onKeyPressed(sf::Event::KeyEvent& event, EventContext context)
{
	return{ false, false };
}
SfmlEventHandler::EventResult SfmlEventHandler::onKeyReleased(sf::Event::KeyEvent& event, EventContext context)
{
	return{ false, false };
}
SfmlEventHandler::EventResult SfmlEventHandler::onMouseWheelMoved(sf::Event::MouseWheelEvent& event, EventContext context)
{
	return{ false, false };
}
SfmlEventHandler::EventResult SfmlEventHandler::onMouseButtonPressed(sf::Event::MouseButtonEvent& event, EventContext context)
{
	return{ false, false };
}
SfmlEventHandler::EventResult SfmlEventHandler::onMouseButtonReleased(sf::Event::MouseButtonEvent& event, EventContext context)
{
	return{ false, false };
}
SfmlEventHandler::EventResult SfmlEventHandler::onMouseMoved(sf::Event::MouseMoveEvent& event, EventContext context)
{
	return{ false, false };
}