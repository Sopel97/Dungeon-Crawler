#pragma once

#include "EntityRenderer.h"

#include "configuration/Configuration.h"

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
class Player;
class Spritesheet;

class PlayerRenderer : public EntityRenderer
{
public:
    PlayerRenderer(Entity& owner);
    PlayerRenderer(Player& player, Entity& owner);
    PlayerRenderer(const PlayerRenderer& other, Entity& owner);
    ~PlayerRenderer() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    void draw(SpriteBatch& mainSpriteBatch, SpriteBatch& metaSpriteBatch) const override;

    const sf::Texture& texture() const;

    std::unique_ptr<EntityRenderer> clone(Entity& owner) const override;
protected:
    Player* m_playerOwner;
    ResourceHandle<Spritesheet> m_spritesheet;
    ls::Vec2I m_sprites;

    EntityModel::Direction direction() const;
};
