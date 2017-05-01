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

	virtual EventResult onTextEntered(sf::Event::TextEvent& event, EventContext context);
    virtual EventResult onKeyPressed(sf::Event::KeyEvent& event, EventContext context);
    virtual EventResult onKeyReleased(sf::Event::KeyEvent& event, EventContext context);
    virtual EventResult onMouseWheelMoved(sf::Event::MouseWheelEvent& event, EventContext context);
    virtual EventResult onMouseButtonPressed(sf::Event::MouseButtonEvent& event, EventContext context);
    virtual EventResult onMouseButtonReleased(sf::Event::MouseButtonEvent& event, EventContext context);
    virtual EventResult onMouseMoved(sf::Event::MouseMoveEvent& event, EventContext context);
};

