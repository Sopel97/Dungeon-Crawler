#pragma once

#include <optional>

#include "window/WindowContent.h"
#include "TileDescription.h"

class WindowSpaceManager;

class TileDescriptionRenderer : public WindowContent
{
private:
    std::optional<TileDescription> m_description;
    WindowSpaceManager* m_wsm;

public:
    TileDescriptionRenderer(WindowSpaceManager& wsm);

    void setDescription(const TileDescription& newDescription);
    void setDescription(TileDescription&& newDescription);
    void removeDescription();

    WindowParams requiredWindowParams() const;
    ls::Rectangle2I requiredContentRect(const ls::Rectangle2I& space) const;

    EventResult onMouseButtonPressed(sf::Event::MouseButtonEvent& event, EventContext context) override;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) override;

private:
    ls::Vec2I calculateContentSize() const;
};