#pragma once

#include <memory>

#include <SFML/Window/Event.hpp>

class Root;
class Game;

//TODO: remove this class entirely and move all reactions to respective classes broadcast events and other classes react to it.
class UserInputHandler
{
public:
    UserInputHandler(Root& root, Game& game);
    ~UserInputHandler();

    void onMouseButtonPressed(sf::Event::MouseButtonEvent& event);
    void onMouseButtonReleased(sf::Event::MouseButtonEvent& event);
    void onMouseMoved(sf::Event::MouseMoveEvent& event);

protected:
    class InputState
    {
    public:
        InputState(UserInputHandler& inputHandler, Root& root, Game& game);
        virtual ~InputState();

        virtual std::unique_ptr<InputState> onMouseButtonPressed(sf::Event::MouseButtonEvent& event);
        virtual std::unique_ptr<InputState> onMouseButtonReleased(sf::Event::MouseButtonEvent& event);
        virtual std::unique_ptr<InputState> onMouseMoved(sf::Event::MouseMoveEvent& event);

    protected:
        UserInputHandler& m_inputHandler;
        Root& m_root;
        Game& m_game;
    };

    class InitialInputState : public InputState
    {
    public:
        InitialInputState(UserInputHandler& inputHandler, Root& root, Game& game);
        virtual ~InitialInputState();

        virtual std::unique_ptr<InputState> onMouseButtonPressed(sf::Event::MouseButtonEvent& event);
        virtual std::unique_ptr<InputState> onMouseButtonReleased(sf::Event::MouseButtonEvent& event);
        virtual std::unique_ptr<InputState> onMouseMoved(sf::Event::MouseMoveEvent& event);
    };

    Root& m_root;
    Game& m_game;
    std::unique_ptr<InputState> m_currentInputState;
};

