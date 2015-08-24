#include "InventorySlotView.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace ls;

ResourceHandle<sf::Texture> InventorySlotView::m_texture {nullptr};
Vec2I InventorySlotView::m_slotTexture {0, 19};
Vec2I InventorySlotView::m_slotTextureSize {40, 40};
Vec2I InventorySlotView::m_requirementIconSize {32, 32};
std::map<InventorySlotView::ContentRequirement, ls::Vec2I> InventorySlotView::m_requirementIcons
{
    {InventorySlotView::ContentRequirement::Necklace, Vec2I(40 + 32 * 0, 19)},
    {InventorySlotView::ContentRequirement::Helmet, Vec2I(40 + 32 * 1, 19)},
    {InventorySlotView::ContentRequirement::Container, Vec2I(40 + 32 * 2, 19)},
    {InventorySlotView::ContentRequirement::Sword, Vec2I(40 + 32 * 3, 19)},
    {InventorySlotView::ContentRequirement::Shield, Vec2I(40 + 32 * 4, 19)},

    {InventorySlotView::ContentRequirement::Chestplate, Vec2I(40 + 32 * 0, 19 + 32)},
    {InventorySlotView::ContentRequirement::Pants, Vec2I(40 + 32 * 1, 19 + 32)},
    {InventorySlotView::ContentRequirement::Ring, Vec2I(40 + 32 * 2, 19 + 32)},
    {InventorySlotView::ContentRequirement::Ammo, Vec2I(40 + 32 * 3, 19 + 32)},
    {InventorySlotView::ContentRequirement::Boots, Vec2I(40 + 32 * 4, 19 + 32)}
};

InventorySlotView::InventorySlotView(Tile*& content, const Vec2I& position, ContentRequirement requirement) :
    m_content(content),
    m_position(position),
    m_contentRequirement(requirement)
{
    if(!m_texture) m_texture = ResourceManager::instance().get<sf::Texture>("UiNonRepeating");
}

bool InventorySlotView::setContent(Tile* newContent)
{
    if(isValidContent(newContent))
    {
        if(m_content != nullptr) delete m_content;
        m_content = newContent;
        return true;
    }
    else return false;
}
bool InventorySlotView::isValidContent(Tile* tile) const
{
    return true; //later compare with requirements
}
bool InventorySlotView::isEmpty() const
{
    return m_content == nullptr;
}
Tile const* InventorySlotView::content() const
{
    return m_content;
}
Tile* InventorySlotView::releaseContent()
{
    Tile* content = m_content;
    m_content = nullptr;
    return content;
}
const Vec2I& InventorySlotView::position() const
{
    return m_position;
}

void InventorySlotView::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    sf::Sprite slotSprite;
    slotSprite.setPosition(m_position.x, m_position.y);
    slotSprite.setTexture(m_texture.get());
    slotSprite.setTextureRect(sf::IntRect(sf::Vector2i(m_slotTexture.x, m_slotTexture.y), sf::Vector2i(m_slotTextureSize.x, m_slotTextureSize.y)));
    renderTarget.draw(slotSprite, renderStates);

    if(m_contentRequirement != ContentRequirement::None)
    {
        Vec2I iconSpritePosition = m_requirementIcons[m_contentRequirement];
        Vec2I iconOffset = (m_slotTextureSize - m_requirementIconSize) / 2;

        sf::Sprite requirementIconSprite;
        requirementIconSprite.setPosition(m_position.x + iconOffset.x, m_position.y + iconOffset.y);
        requirementIconSprite.setTexture(m_texture.get());
        requirementIconSprite.setTextureRect(sf::IntRect(sf::Vector2i(iconSpritePosition.x, iconSpritePosition.y), sf::Vector2i(m_requirementIconSize.x, m_requirementIconSize.y)));
        renderTarget.draw(requirementIconSprite, renderStates);
    }
}

const Vec2I& InventorySlotView::slotSize()
{
    return m_slotTextureSize;
}
