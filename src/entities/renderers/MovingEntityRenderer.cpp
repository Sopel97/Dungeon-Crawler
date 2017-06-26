#include "entities/renderers/MovingEntityRenderer.h"

#include "entities/Entity.h"
#include "entities/models/EntityModel.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "GameConstants.h"

using namespace ls;

REGISTER_ENTITY_RENDERER_TYPE(MovingEntityRenderer)

MovingEntityRenderer::MovingEntityRenderer(Entity& owner, ComponentCommonData* commonData) :
    EntityRenderer(owner),
    m_texture(nullptr),
    m_metaTexture(nullptr),
    m_sprites(0, 0)
{

}

MovingEntityRenderer::MovingEntityRenderer(const MovingEntityRenderer& other, Entity& owner) :
    EntityRenderer(other, owner),
    m_sprites(other.m_sprites)
{

}
MovingEntityRenderer::~MovingEntityRenderer()
{

}

void MovingEntityRenderer::loadFromConfiguration(ConfigurationNode& config)
{
    m_texture = ResourceManager::instance().get<sf::Texture>(config["texture"].get<std::string>());
    m_metaTexture = ResourceManager::instance().get<sf::Texture>(config["metaTexture"].get<std::string>());
    m_sprites.x = config["sprites"][1].get<int>();
    m_sprites.y = config["sprites"][2].get<int>();
}

void MovingEntityRenderer::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const
{
    draw(renderTarget, renderStates, m_texture.get());
}

void MovingEntityRenderer::drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const
{
    draw(renderTarget, renderStates, m_metaTexture.get());
}
void MovingEntityRenderer::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const sf::Texture& texture) const
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
    spr.setTexture(texture);
    spr.setTextureRect(sf::IntRect(sf::Vector2i(m_sprites.x + steppingSpriteVariant * GameConstants::tileFullSpriteSize, m_sprites.y + direction * GameConstants::tileFullSpriteSize), sf::Vector2i(spriteSize.x, spriteSize.y)));
    renderTarget.draw(spr, renderStates);
}

const sf::Texture& MovingEntityRenderer::texture() const
{
    return m_texture.get();
}

std::unique_ptr<EntityRenderer> MovingEntityRenderer::clone(Entity& owner) const
{
    return std::make_unique<MovingEntityRenderer>(*this, owner);
}
