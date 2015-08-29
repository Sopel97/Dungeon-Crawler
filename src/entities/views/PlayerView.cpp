#include "PlayerView.h"

#include "Entity.h"
#include "EntityModel.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>



#include "GameConstants.h"

using namespace ls;

PlayerView::PlayerView(Entity* owner, Player* player) :
    EntityView(owner),
    m_playerOwner(player),
    m_texture(ResourceManager::instance().get<sf::Texture>("Spritesheet")),
    m_sprites(GameConstants::tileFullSpriteSize * 5, GameConstants::tileFullSpriteSize * 2)
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
    const Vec2I spriteOffset = Vec2I(-25, -26);
    constexpr float steppingSpeedThreshold = 16.0f;

    const auto& model = m_owner->model();
    const int direction = model.directionOfMove();
    const float speed = model.velocity().magnitude();
    int steppingSpriteVariant = 0;

    if(speed > steppingSpeedThreshold)
    {
        constexpr float distanceTravelledPerStep = 16.0f;
        constexpr int numberOfSteppingSprites = 2;
        const float distanceTravelled = model.distanceTravelled();

        steppingSpriteVariant = static_cast<int>(distanceTravelled / distanceTravelledPerStep) % numberOfSteppingSprites + 1;
    }

    const Vec2F position = m_owner->model().position();
    const Vec2I spriteSize(GameConstants::tileSize, GameConstants::tileSize);
    sf::Sprite spr;
    spr.setPosition(sf::Vector2f(position.x + spriteOffset.x, position.y + spriteOffset.y));
    spr.setTexture(texture());
    spr.setTextureRect(sf::IntRect(sf::Vector2i(m_sprites.x + steppingSpriteVariant * GameConstants::tileFullSpriteSize, m_sprites.y + direction * GameConstants::tileFullSpriteSize), sf::Vector2i(spriteSize.x, spriteSize.y)));
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
