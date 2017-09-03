#pragma once

#include "EntityRenderer.h"

#include "Configuration.h"

#include "ResourceLoaders.h"

#include <memory>

#include "../LibS/Geometry.h"

namespace sf
{
    class RenderTarget;
    class RenderStates;
    class Texture;
}
class Entity;

class MovingEntityRenderer : public EntityRenderer
{
public:
    MovingEntityRenderer(Entity& owner);
    MovingEntityRenderer(const MovingEntityRenderer& other, Entity& owner);
    ~MovingEntityRenderer() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    void draw(SpriteBatch& mainSpriteBatch, SpriteBatch& metaSpriteBatch) const override;

    const sf::Texture& texture() const;

    std::unique_ptr<EntityRenderer> clone(Entity& owner) const override;
protected:
    ResourceHandle<Spritesheet> m_spritesheet;
    ls::Vec2I m_sprites;
    bool m_hasMetaTexture;

    void draw(SpriteBatch& spriteBatch, const ls::Vec2I& textureOffset) const;
};
