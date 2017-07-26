#include "InventorySlotView.h"

#include "tiles/Tile.h"
#include "tiles/TileStack.h"
#include "tiles/renderers/TileRenderer.h"

#include "Inventory.h"

#include "sprite/Spritesheet.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace ls;

ResourceHandle<Spritesheet> InventorySlotView::m_spritesheet{nullptr};
ResourceHandle<sf::Font> InventorySlotView::m_font{ nullptr };
Vec2I InventorySlotView::m_slotTexture {0, 19};
Vec2I InventorySlotView::m_slotTextureSize {38, 38};
Vec2I InventorySlotView::m_requirementIconSize {32, 32};
std::map<SlotContentRequirement, ls::Vec2I> InventorySlotView::m_requirementIcons
{
    {SlotContentRequirement::Necklace, Vec2I(38 + 32 * 0, 19)},
    {SlotContentRequirement::Helmet, Vec2I(38 + 32 * 1, 19)},
    {SlotContentRequirement::Container, Vec2I(38 + 32 * 2, 19)},
    {SlotContentRequirement::RightHand, Vec2I(38 + 32 * 3, 19)},
    {SlotContentRequirement::LeftHand, Vec2I(38 + 32 * 4, 19)},

    {SlotContentRequirement::Chestplate, Vec2I(38 + 32 * 0, 19 + 32)},
    {SlotContentRequirement::Pants, Vec2I(38 + 32 * 1, 19 + 32)},
    {SlotContentRequirement::Ring, Vec2I(38 + 32 * 2, 19 + 32)},
    {SlotContentRequirement::Ammo, Vec2I(38 + 32 * 3, 19 + 32)},
    {SlotContentRequirement::Boots, Vec2I(38 + 32 * 4, 19 + 32)}
};

InventorySlotView::InventorySlotView(Inventory* inventory, size_t slotId, const ls::Vec2I& position) :
    m_inventory(inventory),
    m_slotId(slotId),
    m_position(position)
{
    if (!m_spritesheet) m_spritesheet = ResourceManager<Spritesheet>::instance().get("UiNonRepeating");
    if (!m_font) m_font = ResourceManager<sf::Font>::instance().get("Font");
}
TileStack& InventorySlotView::content()
{
    return m_inventory->at(m_slotId);
}
const TileStack& InventorySlotView::content() const
{
    return m_inventory->at(m_slotId);
}
const Vec2I& InventorySlotView::position() const
{
    return m_position;
}
ls::Vec2I InventorySlotView::center() const
{
    return m_position + m_slotTextureSize / 2;
}
size_t InventorySlotView::slotId() const
{
    return m_slotId;
}
Inventory& InventorySlotView::inventory()
{
    return *m_inventory;
}
const Inventory& InventorySlotView::inventory() const
{
    return *m_inventory;
}

void InventorySlotView::draw(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates)
{
    sf::Sprite slotSprite;
    slotSprite.setPosition(static_cast<float>(m_position.x), static_cast<float>(m_position.y));
    slotSprite.setTexture(m_spritesheet.get().texture());
    slotSprite.setTextureRect(sf::IntRect(sf::Vector2i(m_slotTexture.x, m_slotTexture.y), sf::Vector2i(m_slotTextureSize.x, m_slotTextureSize.y)));
    renderTarget.draw(slotSprite, renderStates);

    const TileStack& tileStack = m_inventory->at(m_slotId);
    const SlotContentRequirement contentRequirement = m_inventory->slotContentRequirement(m_slotId);
    if(contentRequirement != SlotContentRequirement::None && tileStack.isEmpty())
    {
        Vec2I iconSpritePosition = m_requirementIcons[contentRequirement];
        Vec2I iconOffset = (m_slotTextureSize - m_requirementIconSize) / 2;

        sf::Sprite requirementIconSprite;
        requirementIconSprite.setPosition(static_cast<float>(m_position.x + iconOffset.x), static_cast<float>(m_position.y + iconOffset.y));
        requirementIconSprite.setTexture(m_spritesheet.get().texture());
        requirementIconSprite.setTextureRect(sf::IntRect(sf::Vector2i(iconSpritePosition.x, iconSpritePosition.y), sf::Vector2i(m_requirementIconSize.x, m_requirementIconSize.y)));
        renderTarget.draw(requirementIconSprite, renderStates);
    }
    
    if (!tileStack.isEmpty())
    {
        tileStack.tile().renderer().draw(renderTarget, renderStates, *this);
        if (tileStack.quantity() > 1)
        {
            drawQuantity(renderTarget, renderStates, tileStack.quantity());
        }
    }
}

const Vec2I& InventorySlotView::slotSize()
{
    return m_slotTextureSize;
}

void InventorySlotView::drawQuantity(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates, int quantity)
{
    constexpr int padding = 2;

    sf::Text quantityText(sf::String(std::to_string(quantity)), m_font.get(), m_fontSize);
    const auto rect = quantityText.getLocalBounds();
    quantityText.setOrigin(rect.left + rect.width, rect.top + rect.height);
    quantityText.setPosition(m_position.x + slotSize().x - padding, m_position.y + slotSize().y - padding);
    renderTarget.draw(quantityText);
}
