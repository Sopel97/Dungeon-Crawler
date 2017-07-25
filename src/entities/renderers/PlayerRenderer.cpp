#include "entities/renderers/PlayerRenderer.h"

#include "entities/Entity.h"
#include "entities/models/EntityModel.h"

#include "SpriteBatch.h"

#include "World.h"

#include "sprite/Spritesheet.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

using namespace ls;

REGISTER_ENTITY_RENDERER_TYPE(PlayerRenderer)

PlayerRenderer::PlayerRenderer(Entity& owner) :
    EntityRenderer(owner),
    m_playerOwner(nullptr),
    m_spritesheet(ResourceManager<Spritesheet>::instance().get("Spritesheet")),
    m_sprites(5, 2)
{

}

PlayerRenderer::PlayerRenderer(Player& player, Entity& owner) :
    EntityRenderer(owner),
    m_playerOwner(&player),
    m_spritesheet(ResourceManager<Spritesheet>::instance().get("Spritesheet")),
    m_sprites(5, 2)
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
    draw(spriteBatch, ls::Vec2I(0, 0));
}

void PlayerRenderer::drawMeta(SpriteBatch& spriteBatch) const
{
    draw(spriteBatch, ls::Vec2I(m_spritesheet.get().texture().getSize().x/2, 0));
}
void PlayerRenderer::draw(SpriteBatch& spriteBatch, const ls::Vec2I& textureOffset) const
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

    const ls::Vec2F sprite = static_cast<ls::Vec2F>(
        m_spritesheet.get().gridCoordsToTexCoords(
            ls::Vec2I(
                m_sprites.x + steppingSpriteVariant,
                m_sprites.y + direction
            )
        )
    );
    const ls::Vec2F spriteSize = static_cast<ls::Vec2F>(m_spritesheet.get().gridSizeToTexSize({ 1, 1 }));
    const ls::Vec2F size(World::tileSize, World::tileSize);
    ls::Vec2F pos = m_owner->model().position() + offsetToOrigin;
    pos.x = std::floor(pos.x);
    pos.y = std::floor(pos.y);

    spriteBatch.emplaceRectangle(&(texture()), pos, size, sprite + textureOffset, spriteSize);
}

const sf::Texture& PlayerRenderer::texture() const
{
    return m_spritesheet.get().texture();
}

std::unique_ptr<EntityRenderer> PlayerRenderer::clone(Entity& owner) const
{
    return std::make_unique<PlayerRenderer>(*this, owner);
}
