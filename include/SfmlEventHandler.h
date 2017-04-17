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

public:
	SfmlEventHandler() = default;
	virtual ~SfmlEventHandler() = default;

	EventResult onTextEntered(sf::Event::TextEvent& event);
	EventResult onKeyPressed(sf::Event::KeyEvent& event);
	EventResult onKeyReleased(sf::Event::KeyEvent& event);
	EventResult onMouseWheelMoved(sf::Event::MouseWheelEvent& event);
	EventResult onMouseButtonPressed(sf::Event::MouseButtonEvent& event);
	EventResult onMouseButtonReleased(sf::Event::MouseButtonEvent& event);
	EventResult onMouseMoved(sf::Event::MouseMoveEvent& event);
};

