#include "entities/renderers/MovingEntityRenderer.h"

#include "entities/Entity.h"
#include "entities/models/EntityModel.h"

#include "SpriteBatch.h"

#include "World.h"

#include "ConfigurationShapesLoaders.h"
#include "ConfigurationResourceHandleLoaders.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

using namespace ls;

REGISTER_ENTITY_RENDERER_TYPE(MovingEntityRenderer)

MovingEntityRenderer::MovingEntityRenderer(Entity& owner) :
    EntityRenderer(owner),
    m_spritesheet(nullptr),
    m_sprites(0, 0)
{

}

MovingEntityRenderer::MovingEntityRenderer(const MovingEntityRenderer& other, Entity& owner) :
    EntityRenderer(other, owner),
    m_spritesheet(other.m_spritesheet),
    m_sprites(other.m_sprites)
{

}
MovingEntityRenderer::~MovingEntityRenderer()
{

}

void MovingEntityRenderer::loadFromConfiguration(ConfigurationNode& config)
{
    ConfigurationLoaders::load(m_spritesheet, config["texture"]);
    m_hasMetaTexture = config["hasMetaTexture"].get<bool>();
    ConfigurationLoaders::load(m_sprites, config["sprites"]);
}

void MovingEntityRenderer::draw(SpriteBatch& mainSpriteBatch, SpriteBatch& metaSpriteBatch) const
{
    const Vec2F offsetToOrigin = Vec2F(-25 / 32.0f, -26 / 32.0f);
    constexpr float steppingSpeedThreshold = 16.0f;

    const auto& model = m_owner->model();
    const int direction = model.directionOfMove();
    const float speed = model.velocity().magnitude();
    int steppingSpriteVariant = 0;

    if (speed > steppingSpeedThreshold)
    {
        constexpr float distanceTravelledPerStep = 16.0f / 32.0f;
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
    const ls::Vec2F size(1.0f, 1.0f);
    ls::Vec2F pos = m_owner->model().position() + offsetToOrigin;

    auto geometry = SpriteBatch::geometryFromRectangle(pos, size, sprite, spriteSize);

    mainSpriteBatch.emplaceGeometry(&(texture()), geometry);

    const sf::Vector2f texOffset(m_spritesheet.get().texture().getSize().x / 2.0f, 0.0f);
    for (auto& v : geometry.vertices)
    {
        v.texCoords += texOffset;
    }
    metaSpriteBatch.emplaceGeometry(&(texture()), geometry);
}

const sf::Texture& MovingEntityRenderer::texture() const
{
    return m_spritesheet.get().texture();
}

std::unique_ptr<EntityRenderer> MovingEntityRenderer::clone(Entity& owner) const
{
    return std::make_unique<MovingEntityRenderer>(*this, owner);
}
