#pragma once 

#include "ComponentCommonData.h"
#include "projectiles/renderers/ProjectileRenderer.h"
#include "sprite/Spritesheet.h"
#include "sprite/TimeAnimatedSprite.h"

#include "ResourceManager.h"

#include "SpriteBatch.h"

#include <memory>

class Projectile;

class OrientedProjectileRenderer : public ProjectileRenderer
{
public:
    struct CommonData : public ComponentCommonData
    {
        ResourceHandle<Spritesheet> spritesheet;
        TimeAnimatedSprite sprites;
        TimeAnimatedSprite shadowSprites;
        bool hasMetaTexture;
        ls::Vec2I offsetToSpriteOrigin;
        float altitude;
    };

    OrientedProjectileRenderer(Projectile& owner, CommonData& commonData);
    OrientedProjectileRenderer(const OrientedProjectileRenderer& other, Projectile& owner);
    ~OrientedProjectileRenderer() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    virtual void draw(SpriteBatch& mainSpriteBatch, SpriteBatch& metaSpriteBatch) const;

    std::unique_ptr<ProjectileRenderer> clone(Projectile& owner) const override;

protected:
    CommonData* const m_commonData;

    SpriteBatch::SpriteGeometry geometry() const;
    SpriteBatch::SpriteGeometry shadowGeometry() const;
    SpriteBatch::SpriteGeometry spriteGeometry(const ls::Vec2I& sprite, float altitude, const sf::Color& color = sf::Color::White) const;

    const sf::Texture& texture() const;
};
