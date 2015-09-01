#pragma once

#include <memory>

#include <SFML/Window/Event.hpp>

class Game;

class UserInputHandler
{
public:
    UserInputHandler(Game& game);
    ~UserInputHandler();

protected:
    class InputState
    {
    public:
        InputState(UserInputHandler& inputHandler, Game& game);
        virtual ~InputState();

        virtual std::unique_ptr<InputState> onMouseButtonPressed(sf::Event::MouseButtonEvent& event);
        virtual std::unique_ptr<InputState> onMouseButtonReleased(sf::Event::MouseButtonEvent& event);
        virtual std::unique_ptr<InputState> onMouseMoved(sf::Event::MouseMoveEvent& event);

    protected:
        UserInputHandler& m_inputHandler;
        Game& m_game;
    };

    Game& m_game;
    std::unique_ptr<InputState> m_currentInputState;
};

