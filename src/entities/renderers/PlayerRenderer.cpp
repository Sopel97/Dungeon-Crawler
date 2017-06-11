#include "entities/renderers/PlayerRenderer.h"

#include "entities/Entity.h"
#include "entities/models/EntityModel.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "GameConstants.h"

using namespace ls;

PlayerRenderer::PlayerRenderer() :
    EntityRenderer(),
    m_playerOwner(nullptr),
    m_texture(),
    m_sprites(GameConstants::tileFullSpriteSize * 5, GameConstants::tileFullSpriteSize * 2),
    m_metaSprites(GameConstants::tileFullSpriteSize * 4, GameConstants::tileFullSpriteSize * 8)
{

}

PlayerRenderer::PlayerRenderer(Player* player) :
    EntityRenderer(),
    m_playerOwner(player),
    m_texture(ResourceManager::instance().get<sf::Texture>("Spritesheet")),
    m_sprites(GameConstants::tileFullSpriteSize * 5, GameConstants::tileFullSpriteSize * 2),
    m_metaSprites(GameConstants::tileFullSpriteSize * 4, GameConstants::tileFullSpriteSize * 8)
{

}
PlayerRenderer::PlayerRenderer(const PlayerRenderer& other) :
    EntityRenderer(other),
    m_playerOwner(other.m_playerOwner),
    m_sprites(other.m_sprites)
{

}
PlayerRenderer::~PlayerRenderer()
{

}

void PlayerRenderer::loadFromConfiguration(ConfigurationNode& config)
{
}

void PlayerRenderer::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const
{
    draw(renderTarget, renderStates, m_sprites);
}

void PlayerRenderer::drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const
{
    draw(renderTarget, renderStates, m_metaSprites);
}
void PlayerRenderer::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const ls::Vec2I& sprites) const
{
    const Vec2I offsetToOrigin = Vec2I(-25, -26);
    constexpr float steppingSpeedThreshold = 16.0f;

    const auto& model = m_owner->model();
    const int direction = model.directionOfMove();
    const float speed = model.velocity().magnitude();
    int steppingSpriteVariant = 0;

    if (speed > steppingSpeedThreshold)
    {
        constexpr float distanceTravelledPerStep = 16.0f;
        constexpr int numberOfSteppingSprites = 2;
        const float distanceTravelled = model.distanceTravelled();

        steppingSpriteVariant = static_cast<int>(distanceTravelled / distanceTravelledPerStep) % numberOfSteppingSprites + 1;
    }

    Vec2F position = m_owner->model().position();
    position.x = std::floor(position.x);
    position.y = std::floor(position.y);
    const Vec2I spriteSize(GameConstants::tileSize, GameConstants::tileSize);
    sf::Sprite spr;
    spr.setPosition(sf::Vector2f(position.x + offsetToOrigin.x, position.y + offsetToOrigin.y));
    spr.setTexture(texture());
    spr.setTextureRect(sf::IntRect(sf::Vector2i(sprites.x + steppingSpriteVariant * GameConstants::tileFullSpriteSize, sprites.y + direction * GameConstants::tileFullSpriteSize), sf::Vector2i(spriteSize.x, spriteSize.y)));
    renderTarget.draw(spr, renderStates);
}

const sf::Texture& PlayerRenderer::texture() const
{
    return m_texture.get();
}

std::unique_ptr<EntityRenderer> PlayerRenderer::clone() const
{
    return std::make_unique<PlayerRenderer>(*this);
}
