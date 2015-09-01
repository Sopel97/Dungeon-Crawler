#include "..\include\UserInputHandler.h"

UserInputHandler::InputState::InputState(UserInputHandler& inputHandler, Game& game) :
    m_inputHandler(inputHandler),
    m_game(game)
{
}
UserInputHandler::InputState::~InputState()
{ 
}

std::unique_ptr<UserInputHandler::InputState> UserInputHandler::InputState::onMouseButtonPressed(sf::Event::MouseButtonEvent& event)
{


    return nullptr;
}
std::unique_ptr<UserInputHandler::InputState> UserInputHandler::InputState::onMouseButtonReleased(sf::Event::MouseButtonEvent& event)
{


    return nullptr;
}
std::unique_ptr<UserInputHandler::InputState> UserInputHandler::InputState::onMouseMoved(sf::Event::MouseMoveEvent& event)
{


    return nullptr;
}


UserInputHandler::UserInputHandler(Game& game) :
    m_game(game)
{
}


UserInputHandler::~UserInputHandler()
{
}
