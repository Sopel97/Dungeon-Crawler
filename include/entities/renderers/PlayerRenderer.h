#pragma once

#include "EntityRenderer.h"

#include "Configuration.h"

#include "ResourceManager.h"

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
    PlayerRenderer(Entity* owner, Player* player);
    PlayerRenderer(const PlayerRenderer& other);
    virtual ~PlayerRenderer();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const;
    virtual void drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const;

    const sf::Texture& texture() const;

    virtual std::unique_ptr<EntityRenderer> clone() const;
    virtual std::unique_ptr<EntityRenderer> create(Entity* owner) const;
protected:
    Player* m_playerOwner;
    ResourceHandle<sf::Texture> m_texture;
    ls::Vec2I m_sprites;
    ls::Vec2I m_metaSprites;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const ls::Vec2I& sprites) const;
};
