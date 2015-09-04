#include "UserInputHandler.h"

#include "Root.h"
#include "Game.h"
#include "World.h"

#include "WindowSpaceManager.h"

#include "../LibS/Geometry.h"

using namespace ls;

UserInputHandler::InputState::InputState(UserInputHandler& inputHandler, Root& root, Game& game) :
    m_inputHandler(inputHandler),
    m_root(root),
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


UserInputHandler::InitialInputState::InitialInputState(UserInputHandler& inputHandler, Root& root, Game& game) :
    InputState(inputHandler, root, game)
{

}
UserInputHandler::InitialInputState::~InitialInputState()
{

}

std::unique_ptr<UserInputHandler::InputState> UserInputHandler::InitialInputState::onMouseButtonPressed(sf::Event::MouseButtonEvent& event)
{
    if(event.button == sf::Mouse::Button::Right)
    {
        const RectangleI& worldViewRect = m_root.windowSpaceManager().regionRect(WindowSpaceManager::Region::Id::World);
        if(Intersections::intersection(worldViewRect, Vec2I(event.x, event.y)))
        {
            const Vec2I tilePosition = m_game.world().screenToTile(Vec2I(event.x, event.y));
            m_game.world().useTile(tilePosition);
        }
    }

    return nullptr;
}
std::unique_ptr<UserInputHandler::InputState> UserInputHandler::InitialInputState::onMouseButtonReleased(sf::Event::MouseButtonEvent& event)
{


    return nullptr;
}
std::unique_ptr<UserInputHandler::InputState> UserInputHandler::InitialInputState::onMouseMoved(sf::Event::MouseMoveEvent& event)
{


    return nullptr;
}



UserInputHandler::UserInputHandler(Root& root, Game& game) :
    m_root(root),
    m_game(game),
    m_currentInputState(std::make_unique<InitialInputState>(*this, root, game))
{
}


UserInputHandler::~UserInputHandler()
{
}

void UserInputHandler::onMouseButtonPressed(sf::Event::MouseButtonEvent& event)
{
    std::unique_ptr<UserInputHandler::InputState> newInputState = m_currentInputState->onMouseButtonPressed(event);
    if(newInputState != nullptr) m_currentInputState = std::move(newInputState);
}
void UserInputHandler::onMouseButtonReleased(sf::Event::MouseButtonEvent& event)
{
    std::unique_ptr<UserInputHandler::InputState> newInputState = m_currentInputState->onMouseButtonReleased(event);
    if(newInputState != nullptr) m_currentInputState = std::move(newInputState);
}
void UserInputHandler::onMouseMoved(sf::Event::MouseMoveEvent& event)
{
    std::unique_ptr<UserInputHandler::InputState> newInputState = m_currentInputState->onMouseMoved(event);
    if(newInputState != nullptr) m_currentInputState = std::move(newInputState);
}
