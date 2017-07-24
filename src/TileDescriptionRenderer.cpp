#include "TileDescriptionRenderer.h"

#include <iostream>

#include "window/WindowSpaceManager.h"

#include "ResourceManager.h"

TileDescriptionRenderer::TileDescriptionRenderer(WindowSpaceManager& wsm) :
    m_description(std::nullopt),
    m_wsm(&wsm)
{
    m_font = ResourceManager::instance().get<sf::Font>("Font");
}

void TileDescriptionRenderer::setDescription(const TileDescription& newDescription)
{
    m_description = newDescription;
    updateLines();
}
void TileDescriptionRenderer::setDescription(TileDescription&& newDescription)
{
    m_description = std::move(newDescription);
    updateLines();
}
void TileDescriptionRenderer::removeDescription()
{
    m_description.reset();
    updateLines();
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
    static constexpr int offsetX = -20;
    static constexpr int offsetY = -10;

    const ls::Vec2I size = m_contentSize;
    const ls::Vec2I mousePos(sf::Mouse::getPosition(m_wsm->window()).x, sf::Mouse::getPosition(m_wsm->window()).y);
    ls::Rectangle2I rect = ls::Rectangle2I::withSize(mousePos - ls::Vec2I(size.x, 0) + ls::Vec2I(offsetX, offsetY), size.x, size.y);

    if (rect.min.x < space.min.x) rect.translate(ls::Vec2I(space.min.x - rect.min.x, 0));
    if (rect.min.y < space.min.y) rect.translate(ls::Vec2I(0, space.min.y - rect.min.y));

    if (rect.max.x > space.max.x) rect.translate(ls::Vec2I(space.max.x - rect.max.x, 0));
    if (rect.max.y > space.max.y) rect.translate(ls::Vec2I(0, space.max.y - rect.max.y));

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

void TileDescriptionRenderer::draw(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates)
{
    for (const auto& line : m_lines)
    {
        renderTarget.draw(line, renderStates);
    }
}

void TileDescriptionRenderer::updateLines()
{
    m_lines.clear();
    m_contentSize = ls::Vec2I(0, 0);

    if (m_description.has_value())
    {
        const TileDescription& desc = m_description.value();
        m_lines.reserve(desc.numLines());

        for (const auto& line : desc)
        {
            m_lines.emplace_back(sf::String(line.text()), m_font.get(), line.fontSize());

            sf::Text& textLine = m_lines.back();
            textLine.setColor(line.color());
            textLine.setStyle(line.style());
        }

        float maxWidth = 0;
        float height = 0;

        float spaceBetweenLines = 0;
        for (const auto& line : m_lines)
        {
            const auto& rect = line.getLocalBounds();
            maxWidth = std::max(maxWidth, rect.width);
            height += rect.height + spaceBetweenLines;

            spaceBetweenLines = m_spaceBetweenLines;
        }

        m_contentSize = ls::Vec2I(static_cast<int>(maxWidth) + 2 * m_padding, static_cast<int>(height) + 2 * m_padding);

        float posY = m_padding;
        for (auto& line : m_lines)
        {
            const auto& rect = line.getLocalBounds();

            line.setOrigin(
                std::floor(rect.left + rect.width / 2.0f),
                std::floor(rect.top)
            );
            line.setPosition(
                std::floor(m_contentSize.x / 2.0f), 
                std::floor(posY)
            );

            posY += rect.height + m_spaceBetweenLines;
        }
    }
}