#include "TileDescriptionRenderer.h"

#include <iostream>

TileDescriptionRenderer::TileDescriptionRenderer() :
    m_description(std::nullopt)
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
    return ls::Rectangle2I::withSize(ls::Vec2I(0, 0), 10, 10); //TODO: actually compute this and set position near mouse
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