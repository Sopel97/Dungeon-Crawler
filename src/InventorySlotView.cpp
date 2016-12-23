#include "InventorySlotView.h"

#include "tiles/Tile.h"
#include "tiles/TileStack.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace ls;

ResourceHandle<sf::Texture> InventorySlotView::m_texture {nullptr};
Vec2I InventorySlotView::m_slotTexture {0, 19};
Vec2I InventorySlotView::m_slotTextureSize {38, 38};
Vec2I InventorySlotView::m_requirementIconSize {32, 32};
std::map<Inventory::ContentRequirement, ls::Vec2I> InventorySlotView::m_requirementIcons
{
    {Inventory::ContentRequirement::Necklace, Vec2I(38 + 32 * 0, 19)},
    {Inventory::ContentRequirement::Helmet, Vec2I(38 + 32 * 1, 19)},
    {Inventory::ContentRequirement::Container, Vec2I(38 + 32 * 2, 19)},
    {Inventory::ContentRequirement::Sword, Vec2I(38 + 32 * 3, 19)},
    {Inventory::ContentRequirement::Shield, Vec2I(38 + 32 * 4, 19)},

    {Inventory::ContentRequirement::Chestplate, Vec2I(38 + 32 * 0, 19 + 32)},
    {Inventory::ContentRequirement::Pants, Vec2I(38 + 32 * 1, 19 + 32)},
    {Inventory::ContentRequirement::Ring, Vec2I(38 + 32 * 2, 19 + 32)},
    {Inventory::ContentRequirement::Ammo, Vec2I(38 + 32 * 3, 19 + 32)},
    {Inventory::ContentRequirement::Boots, Vec2I(38 + 32 * 4, 19 + 32)}
};

InventorySlotView::InventorySlotView(Inventory* inventory, size_t slotId, const ls::Vec2I& position) :
    m_inventory(inventory),
    m_slotId(slotId),
    m_position(position)
{
    if(!m_texture) m_texture = ResourceManager::instance().get<sf::Texture>("UiNonRepeating");
}
TileStack* InventorySlotView::content() const
{
    return m_inventory->at(m_slotId);
}
const Vec2I& InventorySlotView::position() const
{
    return m_position;
}
void InventorySlotView::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, Inventory::ContentRequirement contentRequirement)
{
    sf::Sprite slotSprite;
    slotSprite.setPosition(static_cast<float>(m_position.x), static_cast<float>(m_position.y));
    slotSprite.setTexture(m_texture.get());
    slotSprite.setTextureRect(sf::IntRect(sf::Vector2i(m_slotTexture.x, m_slotTexture.y), sf::Vector2i(m_slotTextureSize.x, m_slotTextureSize.y)));
    renderTarget.draw(slotSprite, renderStates);

    if(contentRequirement != Inventory::ContentRequirement::None)
    {
        Vec2I iconSpritePosition = m_requirementIcons[contentRequirement];
        Vec2I iconOffset = (m_slotTextureSize - m_requirementIconSize) / 2;

        sf::Sprite requirementIconSprite;
        requirementIconSprite.setPosition(static_cast<float>(m_position.x + iconOffset.x), static_cast<float>(m_position.y + iconOffset.y));
        requirementIconSprite.setTexture(m_texture.get());
        requirementIconSprite.setTextureRect(sf::IntRect(sf::Vector2i(iconSpritePosition.x, iconSpritePosition.y), sf::Vector2i(m_requirementIconSize.x, m_requirementIconSize.y)));
        renderTarget.draw(requirementIconSprite, renderStates);
    }
}

const Vec2I& InventorySlotView::slotSize()
{
    return m_slotTextureSize;
}
