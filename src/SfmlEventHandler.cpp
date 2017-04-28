#include "..\include\SfmlEventHandler.h"


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