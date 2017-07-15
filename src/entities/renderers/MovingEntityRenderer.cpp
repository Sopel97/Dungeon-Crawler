#include "entities/renderers/MovingEntityRenderer.h"

#include "entities/Entity.h"
#include "entities/models/EntityModel.h"

#include "SpriteBatch.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "GameConstants.h"

using namespace ls;

REGISTER_ENTITY_RENDERER_TYPE(MovingEntityRenderer)

MovingEntityRenderer::MovingEntityRenderer(Entity& owner) :
    EntityRenderer(owner),
    m_texture(nullptr),
    m_sprites(0, 0)
{

}

MovingEntityRenderer::MovingEntityRenderer(const MovingEntityRenderer& other, Entity& owner) :
    EntityRenderer(other, owner),
    m_texture(other.m_texture),
    m_sprites(other.m_sprites)
{

}
MovingEntityRenderer::~MovingEntityRenderer()
{

}

void MovingEntityRenderer::loadFromConfiguration(ConfigurationNode& config)
{
    m_texture = ResourceManager::instance().get<sf::Texture>(config["texture"].get<std::string>());
    m_hasMetaTexture = config["hasMetaTexture"].get<bool>();
    m_sprites.x = config["sprites"][1].get<int>();
    m_sprites.y = config["sprites"][2].get<int>();
}

void MovingEntityRenderer::draw(SpriteBatch& spriteBatch) const
{
    draw(spriteBatch, ls::Vec2I(0, 0));
}

void MovingEntityRenderer::drawMeta(SpriteBatch& spriteBatch) const
{
    draw(spriteBatch, ls::Vec2I(m_texture.get().getSize().x / 2, 0));
}
void MovingEntityRenderer::draw(SpriteBatch& spriteBatch, const ls::Vec2I& textureOffset) const
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

    const ls::Vec2F sprite(m_sprites.x + textureOffset.x + steppingSpriteVariant * GameConstants::tileFullSpriteSize, m_sprites.y + textureOffset.y + direction * GameConstants::tileFullSpriteSize);
    const ls::Vec2F size(GameConstants::tileSize, GameConstants::tileSize);
    ls::Vec2F pos = m_owner->model().position() + offsetToOrigin;
    pos.x = std::floor(pos.x);
    pos.y = std::floor(pos.y);

    spriteBatch.emplaceRectangle(&(texture()), pos, sprite, size);
}

const sf::Texture& MovingEntityRenderer::texture() const
{
    return m_texture.get();
}

std::unique_ptr<EntityRenderer> MovingEntityRenderer::clone(Entity& owner) const
{
    return std::make_unique<MovingEntityRenderer>(*this, owner);
}
