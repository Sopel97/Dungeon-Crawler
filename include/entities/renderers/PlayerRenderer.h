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
class Player;

class PlayerRenderer : public EntityRenderer
{
public:
    PlayerRenderer(Entity& owner, ComponentCommonData* commonData);
    PlayerRenderer(Player& player, Entity& owner);
    PlayerRenderer(const PlayerRenderer& other, Entity& owner);
    ~PlayerRenderer() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const override;
    void drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const override;

    const sf::Texture& texture() const;

    std::unique_ptr<EntityRenderer> clone(Entity& owner) const override;
protected:
    Player* m_playerOwner;
    ResourceHandle<sf::Texture> m_texture;
    ls::Vec2I m_sprites;
    ls::Vec2I m_metaSprites;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const ls::Vec2I& sprites) const;
};
