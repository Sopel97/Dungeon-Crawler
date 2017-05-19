#pragma once

#include <SFML/Window/Event.hpp>

#include "../LibS/Shapes.h"

class SfmlEventHandler
{
public:
	struct EventResult
	{
		bool takeFocus;
		bool consumeEvent;
        
        EventResult setTakeFocus(bool opt = true) const;
        EventResult setConsumeEvent(bool opt = true) const;
	};

	struct EventContext
	{
		bool isFocused;
		bool isMouseOver;

        EventContext setIsFocused(bool opt = true) const;
        EventContext setIsMouseOver(bool opt = true) const;
	};

public:
	SfmlEventHandler() = default;
	virtual ~SfmlEventHandler() = default;

    virtual EventResult dispatch(sf::Event& event, EventContext context, const ls::Vec2I& mousePos);

	virtual EventResult onTextEntered(sf::Event::TextEvent& event, EventContext context);
    virtual EventResult onKeyPressed(sf::Event::KeyEvent& event, EventContext context);
    virtual EventResult onKeyReleased(sf::Event::KeyEvent& event, EventContext context);
    virtual EventResult onMouseWheelMoved(sf::Event::MouseWheelEvent& event, EventContext context);
    virtual EventResult onMouseButtonPressed(sf::Event::MouseButtonEvent& event, EventContext context);
    virtual EventResult onMouseButtonReleased(sf::Event::MouseButtonEvent& event, EventContext context);
    virtual EventResult onMouseMoved(sf::Event::MouseMoveEvent& event, EventContext context);
};

