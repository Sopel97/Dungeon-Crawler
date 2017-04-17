#include "..\include\SfmlEventHandler.h"


SfmlEventHandler::EventResult SfmlEventHandler::onTextEntered(sf::Event::TextEvent& event)
{
	return{ false, false };
}
SfmlEventHandler::EventResult SfmlEventHandler::onKeyPressed(sf::Event::KeyEvent& event)
{
	return{ false, false };
}
SfmlEventHandler::EventResult SfmlEventHandler::onKeyReleased(sf::Event::KeyEvent& event)
{
	return{ false, false };
}
SfmlEventHandler::EventResult SfmlEventHandler::onMouseWheelMoved(sf::Event::MouseWheelEvent& event)
{
	return{ false, false };
}
SfmlEventHandler::EventResult SfmlEventHandler::onMouseButtonPressed(sf::Event::MouseButtonEvent& event)
{
	return{ false, false };
}
SfmlEventHandler::EventResult SfmlEventHandler::onMouseButtonReleased(sf::Event::MouseButtonEvent& event)
{
	return{ false, false };
}
SfmlEventHandler::EventResult SfmlEventHandler::onMouseMoved(sf::Event::MouseMoveEvent& event)
{
	return{ false, false };
}