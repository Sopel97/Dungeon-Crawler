#pragma once 

#include "ComponentCommonData.h"
#include "projectiles/renderers/ProjectileRenderer.h"
#include "sprite/Spritesheet.h"
#include "sprite/TimeAnimatedSprite.h"

#include "ResourceManager.h"

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

    void draw(SpriteBatch& spriteBatch) const override;
    void drawMeta(SpriteBatch& spriteBatch) const override;

    std::unique_ptr<ProjectileRenderer> clone(Projectile& owner) const override;

protected:
    CommonData* const m_commonData;

    void draw(SpriteBatch& spriteBatch, const ls::Vec2I& texOffset, const sf::Color& color = sf::Color::White) const;
    void drawShadow(SpriteBatch& spriteBatch) const;

    void drawSprite(SpriteBatch& spriteBatch, const ls::Vec2I& sprite, const ls::Vec2I& texOffset, float altitude, const sf::Color& color = sf::Color::White) const;
};
