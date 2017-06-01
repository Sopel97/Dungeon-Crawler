#pragma once

#include <optional>

#include <SFML/Graphics.hpp>

#include "window/WindowContent.h"
#include "TileDescription.h"

#include "ResourceManager.h"

class WindowSpaceManager;

class TileDescriptionRenderer : public WindowContent
{
private:
    std::optional<TileDescription> m_description;
    WindowSpaceManager* m_wsm;
    ResourceHandle<sf::Font> m_font;
    std::vector<sf::Text> m_lines;
    ls::Vec2I m_contentSize;

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
    void updateLines();

    static constexpr int m_padding = 10;
    static constexpr int m_spaceBetweenLines = 8;
};