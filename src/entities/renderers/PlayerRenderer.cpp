#include "entities/renderers/PlayerRenderer.h"

#include "entities/Entity.h"
#include "entities/models/EntityModel.h"

#include "SpriteBatch.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "GameConstants.h"

using namespace ls;

REGISTER_ENTITY_RENDERER_TYPE(PlayerRenderer)

PlayerRenderer::PlayerRenderer(Entity& owner) :
    EntityRenderer(owner),
    m_playerOwner(nullptr),
    m_texture(ResourceManager::instance().get<sf::Texture>("Spritesheet")),
    m_sprites(GameConstants::tileFullSpriteSize * 5, GameConstants::tileFullSpriteSize * 2)
{

}

PlayerRenderer::PlayerRenderer(Player& player, Entity& owner) :
    EntityRenderer(owner),
    m_playerOwner(&player),
    m_texture(ResourceManager::instance().get<sf::Texture>("Spritesheet")),
    m_sprites(GameConstants::tileFullSpriteSize * 5, GameConstants::tileFullSpriteSize * 2)
{

}
PlayerRenderer::PlayerRenderer(const PlayerRenderer& other, Entity& owner) :
    EntityRenderer(other, owner),
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

void PlayerRenderer::draw(SpriteBatch& spriteBatch) const
{
    draw(spriteBatch, m_sprites);
}

void PlayerRenderer::drawMeta(SpriteBatch& spriteBatch) const
{
    draw(spriteBatch, m_sprites + ls::Vec2I(m_texture.get().getSize().x/2, 0));
}
void PlayerRenderer::draw(SpriteBatch& spriteBatch, const ls::Vec2I& sprites) const
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

    const ls::Vec2F sprite(
        static_cast<float>(sprites.x + steppingSpriteVariant * GameConstants::tileFullSpriteSize), 
        static_cast<float>(sprites.y + direction * GameConstants::tileFullSpriteSize)
    );
    const ls::Vec2F size(static_cast<float>(GameConstants::tileSize), static_cast<float>(GameConstants::tileSize));
    ls::Vec2F pos = m_owner->model().position() + offsetToOrigin;
    pos.x = std::floor(pos.x);
    pos.y = std::floor(pos.y);

    spriteBatch.emplaceRectangle(&(texture()), pos, sprite, size);
}

const sf::Texture& PlayerRenderer::texture() const
{
    return m_texture.get();
}

std::unique_ptr<EntityRenderer> PlayerRenderer::clone(Entity& owner) const
{
    return std::make_unique<PlayerRenderer>(*this, owner);
}
