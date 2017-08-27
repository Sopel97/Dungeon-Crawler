#include "projectiles/renderers/OrientedProjectileRenderer.h"

#include "ResourceLoaders.h"

#include "SpriteBatch.h"

REGISTER_PROJECTILE_RENDERER_TYPE(OrientedProjectileRenderer);

OrientedProjectileRenderer::OrientedProjectileRenderer(Projectile& owner, CommonData& commonData) :
    ProjectileRenderer(owner),
    m_commonData(&commonData)
{

}
OrientedProjectileRenderer::OrientedProjectileRenderer(const OrientedProjectileRenderer& other, Projectile& owner) :
    ProjectileRenderer(other, owner),
    m_commonData(other.m_commonData)
{

}
OrientedProjectileRenderer::~OrientedProjectileRenderer()
{

}

void OrientedProjectileRenderer::loadFromConfiguration(ConfigurationNode& config)
{
    m_commonData->spritesheet = ResourceManager<Spritesheet>::instance().get(config["texture"].get<std::string>());
    m_commonData->sprites.loadFromConfiguration(config["sprites"]);
    m_commonData->shadowSprites.loadFromConfiguration(config["shadowSprites"]);
    m_commonData->hasMetaTexture = config["hasMetaTexture"].get<bool>();
    m_commonData->offsetToSpriteOrigin.x = config["offsetToSpriteOrigin"][1].get<int>();
    m_commonData->offsetToSpriteOrigin.y = config["offsetToSpriteOrigin"][2].get<int>();
    m_commonData->altitude = config["altitude"].getDefault<float>(0.0f);
}

void OrientedProjectileRenderer::draw(SpriteBatch& spriteBatch) const
{
    draw(spriteBatch, ls::Vec2I(0, 0));

    drawShadow(spriteBatch);
}
void OrientedProjectileRenderer::drawMeta(SpriteBatch& spriteBatch) const
{
    if (!m_commonData->hasMetaTexture) return;

    draw(spriteBatch, ls::Vec2I(m_commonData->spritesheet.get().texture().getSize().x / 2, 0));
}
void OrientedProjectileRenderer::drawShadow(SpriteBatch& spriteBatch) const
{
    drawSprite(spriteBatch, m_commonData->shadowSprites.now(), ls::Vec2I(0, 0), 0.0f);
}
void OrientedProjectileRenderer::drawSprite(SpriteBatch& spriteBatch, const ls::Vec2I& sprite, const ls::Vec2I& texOffset, float altitude, const sf::Color& color) const
{
    const ls::Vec2F offset = m_commonData->offsetToSpriteOrigin / 32.0f;

    ls::Vec2F velocity = m_owner->model().velocity();
    if (velocity.magnitude() < 0.01f) velocity.y = 1.0f;

    const ls::Vec2F direction = velocity.normalized();
    const ls::Vec2F pos = m_owner->model().position() - offset - ls::Vec2F(altitude, altitude);

    const ls::Vec2F spritePos = static_cast<ls::Vec2F>(
        m_commonData->spritesheet.get().gridCoordsToTexCoords(ls::Vec2I(sprite.x, sprite.y))
        );
    const ls::Vec2F spriteSize = static_cast<ls::Vec2F>(m_commonData->spritesheet.get().gridSizeToTexSize({ 1, 1 }));
    const ls::Vec2F size(1.0f, 1.0f);

    spriteBatch.emplaceRotatedRectangle(&(m_commonData->spritesheet.get().texture()), pos, size, spritePos + texOffset, spriteSize, direction, pos + offset, color);
}
void OrientedProjectileRenderer::draw(SpriteBatch& spriteBatch, const ls::Vec2I& texOffset, const sf::Color& color) const
{
    drawSprite(spriteBatch, m_commonData->sprites.now(), texOffset, m_commonData->altitude);
}

std::unique_ptr<ProjectileRenderer> OrientedProjectileRenderer::clone(Projectile& owner) const
{
    return std::make_unique<OrientedProjectileRenderer>(*this, owner);
}