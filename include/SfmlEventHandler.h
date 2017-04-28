#pragma once

#include <SFML/Window/Event.hpp>

class SfmlEventHandler
{
public:
	struct EventResult
	{
		bool takeFocus;
		bool consumeEvent;
	};

	struct EventContext
	{
		bool isFocused;
		bool isMouseOver;
	};

public:
	SfmlEventHandler() = default;
	virtual ~SfmlEventHandler() = default;

	EventResult onTextEntered(sf::Event::TextEvent& event, EventContext context);
	EventResult onKeyPressed(sf::Event::KeyEvent& event, EventContext context);
	EventResult onKeyReleased(sf::Event::KeyEvent& event, EventContext context);
	EventResult onMouseWheelMoved(sf::Event::MouseWheelEvent& event, EventContext context);
	EventResult onMouseButtonPressed(sf::Event::MouseButtonEvent& event, EventContext context);
	EventResult onMouseButtonReleased(sf::Event::MouseButtonEvent& event, EventContext context);
	EventResult onMouseMoved(sf::Event::MouseMoveEvent& event, EventContext context);
};

