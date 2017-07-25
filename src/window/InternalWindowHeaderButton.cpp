#include "window/InternalWindowHeaderButton.h"

#include "ResourceManager.h"

#include "sprite/Spritesheet.h"

#include <iostream>

InternalWindowHeaderButton::InternalWindowHeaderButton(InternalWindow& parent, int id, const ls::Rectangle2I& sprite, bool enabled) :
    m_parentWindow(&parent),
    m_spriteRect(sprite),
    m_callback([](InternalWindow&) {}),
    m_id(id),
    m_isEnabled(enabled)
{

}

int InternalWindowHeaderButton::width() const
{
    return m_spriteRect.width();
}
int InternalWindowHeaderButton::height() const
{
    return m_spriteRect.height();
}
int InternalWindowHeaderButton::id() const
{
    return m_id;
}
bool InternalWindowHeaderButton::isEnabled() const
{
    return m_isEnabled;
}

void InternalWindowHeaderButton::makeCallback()
{
    m_callback(*m_parentWindow);
}
void InternalWindowHeaderButton::setEnabled(bool enable)
{
    m_isEnabled = enable;
}

void InternalWindowHeaderButton::draw(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates, const ls::Vec2I& pos)
{
    ResourceHandle<Spritesheet> nonRepeatingSprites = ResourceManager::instance().get<Spritesheet>("UiNonRepeating");

    sf::Sprite button;
    button.setPosition(static_cast<float>(pos.x), static_cast<float>(pos.y));
    button.setTexture(nonRepeatingSprites.get().texture());
    button.setTextureRect(sf::IntRect(sf::Vector2i(m_spriteRect.min.x, m_spriteRect.min.y), sf::Vector2i(m_spriteRect.width(), m_spriteRect.height())));
    renderTarget.draw(button, renderStates);
}