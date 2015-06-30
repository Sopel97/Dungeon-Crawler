#include "PlayerView.h"

#include "Entity.h"
#include "EntityModel.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "../LibS/make_unique.h"

using namespace Geo;

PlayerView::PlayerView(Entity* owner, Player* player) :
    EntityView(owner),
    m_playerOwner(player),
    m_texture(ResourceManager::instance().get<sf::Texture>("Spritesheet")),
    m_sprites(160, 64)
{

}
PlayerView::PlayerView(const PlayerView& other) :
    EntityView(other.m_owner),
    m_playerOwner(other.m_playerOwner),
    m_sprites(other.m_sprites)
{

}
PlayerView::~PlayerView()
{

}

void PlayerView::loadFromConfiguration(ConfigurationNode& config)
{
}

void PlayerView::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const
{
    static const Vec2I spriteOffset = Vec2I(-25, -26);
    const int direction = m_owner->model().directionOfMove();

    const Vec2F position = m_owner->model().position();
    const Vec2I spriteSize(32, 32);
    sf::Sprite spr;
    spr.setPosition(sf::Vector2f(position.x + spriteOffset.x, position.y + spriteOffset.y));
    spr.setTexture(texture());
    spr.setTextureRect(sf::IntRect(sf::Vector2i(m_sprites.x, m_sprites.y + direction * 32), sf::Vector2i(spriteSize.x, spriteSize.y)));
    renderTarget.draw(spr, renderStates);
}

const sf::Texture& PlayerView::texture() const
{
    return m_texture.get();
}

std::unique_ptr<EntityView> PlayerView::clone() const
{
    return std::make_unique<PlayerView>(*this);
}
std::unique_ptr<EntityView> PlayerView::create(Entity* owner) const
{
    return std::make_unique<PlayerView>(owner, nullptr);
}
