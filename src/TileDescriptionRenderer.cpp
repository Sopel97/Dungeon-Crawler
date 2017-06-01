#include "TileDescriptionRenderer.h"

#include <iostream>

#include "window/WindowSpaceManager.h"

TileDescriptionRenderer::TileDescriptionRenderer(WindowSpaceManager& wsm) :
    m_description(std::nullopt),
    m_wsm(&wsm)
{

}

void TileDescriptionRenderer::setDescription(const TileDescription& newDescription)
{
    m_description = newDescription;
}
void TileDescriptionRenderer::setDescription(TileDescription&& newDescription)
{
    m_description = std::move(newDescription);
}
void TileDescriptionRenderer::removeDescription()
{
    m_description.reset();
}

WindowParams TileDescriptionRenderer::requiredWindowParams() const
{
    WindowParams params;

    params.minWindowWidth = 0;
    params.minWindowHeight = 0;
    params.minContentWidth = 0;
    params.minContentHeight = 0;

    params.maxWindowWidth = std::nullopt;
    params.maxWindowHeight = std::nullopt;
    params.maxContentWidth = std::nullopt;
    params.maxContentHeight = std::nullopt;

    params.isMinimizable = false;
    params.isCloseable = true;
    params.isResizeable = false;
    params.isMovable = false;
    params.isContentOnly = false;
    params.hasHeader = false;
    params.hasScrollBar = false;

    return params;
}
ls::Rectangle2I TileDescriptionRenderer::requiredContentRect(const ls::Rectangle2I& space) const
{
    const ls::Vec2I size = calculateContentSize();
    const ls::Vec2I mousePos(sf::Mouse::getPosition(m_wsm->window()).x, sf::Mouse::getPosition(m_wsm->window()).y);
    ls::Rectangle2I rect = ls::Rectangle2I::withSize(mousePos - ls::Vec2I(size.x, 0), size.x, size.y);

    //TODO: position properly if outside window

    return rect;
}
SfmlEventHandler::EventResult TileDescriptionRenderer::onMouseButtonPressed(sf::Event::MouseButtonEvent& event, EventContext context)
{
    if (context.isMouseOver && event.button == sf::Mouse::Button::Left)
    {
        window().close();
        return EventResult{}.setConsumeEvent(true).setTakeFocus(false);
    }

    return EventResult{}.setConsumeEvent(false).setTakeFocus(false);
}

void TileDescriptionRenderer::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    // TODO: drawing
}

ls::Vec2I TileDescriptionRenderer::calculateContentSize() const
{
    // TODO: 

    return ls::Vec2I(100, 100);
}